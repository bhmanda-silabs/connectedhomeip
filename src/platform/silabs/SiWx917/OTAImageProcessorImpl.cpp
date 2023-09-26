/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <app/clusters/ota-requestor/OTADownloader.h>
#include <app/clusters/ota-requestor/OTARequestorInterface.h>
#include <platform/silabs/OTAImageProcessorImpl.h>

#include <platform/silabs/SilabsConfig.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "sl_si91x_driver.h"
#ifdef RSI_M4_INTERFACE
#include "sl_si91x_hal_soc_soft_reset.h"
#endif
#ifdef __cplusplus
}
#endif


#define OTA_HEADER 0
#define RPS_HEADER 1
#define RPS_DATA   2
/// No error, operation OK
#define SL_BOOTLOADER_OK 0L
#define SL_STATUS_FW_UPDATE_DONE ((sl_status_t)0x10003)
uint8_t flag = OTA_HEADER;
uint8_t last = 0;
uint8_t length = 0;
uint32_t total_size = 0;
uint32_t count = 0;
uint32_t loop_counter = 0;

    uint8_t temp_buf[64];
namespace chip {

// Define static memebers
uint8_t OTAImageProcessorImpl::mSlotId                                                  = 0;
uint32_t OTAImageProcessorImpl::mWriteOffset                                            = 0;
uint16_t OTAImageProcessorImpl::writeBufOffset                                          = 0;
uint8_t OTAImageProcessorImpl::writeBuffer[kAlignmentBytes] __attribute__((aligned(4))) = { 0 };

CHIP_ERROR OTAImageProcessorImpl::PrepareDownload()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandlePrepareDownload, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Finalize()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleFinalize, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}
CHIP_ERROR OTAImageProcessorImpl::Apply()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleApply, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::Abort()
{
    DeviceLayer::PlatformMgr().ScheduleWork(HandleAbort, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ProcessBlock(ByteSpan & block)
{
    if ((block.data() == nullptr) || block.empty())
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    // Store block data for HandleProcessBlock to access
    CHIP_ERROR err = SetBlock(block);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot set block data: %" CHIP_ERROR_FORMAT, err.Format());
    }

    DeviceLayer::PlatformMgr().ScheduleWork(HandleProcessBlock, reinterpret_cast<intptr_t>(this));
    return CHIP_NO_ERROR;
}

bool OTAImageProcessorImpl::IsFirstImageRun()
{
    OTARequestorInterface * requestor = chip::GetRequestorInstance();
    if (requestor == nullptr)
    {
        return false;
    }

    return requestor->GetCurrentUpdateState() == OTARequestorInterface::OTAUpdateStateEnum::kApplying;
}

CHIP_ERROR OTAImageProcessorImpl::ConfirmCurrentImage()
{
    OTARequestorInterface * requestor = chip::GetRequestorInstance();
    if (requestor == nullptr)
    {
        return CHIP_ERROR_INTERNAL;
    }

    uint32_t currentVersion;
    uint32_t targetVersion = requestor->GetTargetVersion();
    ReturnErrorOnFailure(DeviceLayer::ConfigurationMgr().GetSoftwareVersion(currentVersion));
    if (currentVersion != targetVersion)
    {
        ChipLogError(SoftwareUpdate, "Current software version = %" PRIu32 ", expected software version = %" PRIu32, currentVersion,
                     targetVersion);
        return CHIP_ERROR_INCORRECT_STATE;
    }

    return CHIP_NO_ERROR;
}

void OTAImageProcessorImpl::HandlePrepareDownload(intptr_t context)
{
    int32_t err           = SL_BOOTLOADER_OK;
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);

    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }

    ChipLogProgress(SoftwareUpdate, "HandlePrepareDownload");

    mSlotId                                 = 0; // Single slot until we support multiple images
    writeBufOffset                          = 0;
    mWriteOffset                            = 0;
    imageProcessor->mParams.downloadedBytes = 0;

    imageProcessor->mHeaderParser.Init();

    // Not calling bootloader_eraseStorageSlot(mSlotId) here because we erase during each write

    imageProcessor->mDownloader->OnPreparedForDownload(CHIP_NO_ERROR);
}

void OTAImageProcessorImpl::HandleFinalize(intptr_t context)
{
    uint32_t err          = SL_BOOTLOADER_OK;
    int32_t status = 0;
	int32_t fin_chunk = 0;
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        return;
    }
    if(last == 1)
    {
        ChipLogProgress(SoftwareUpdate, "OTA image downloaded successfully in HandleFinalize");
        return;
    }
    // Pad the remainder of the write buffer with zeros and write it to bootloader storage
    if (writeBufOffset != 0)
    {
        // Account for last bytes of the image not yet written to storage
        imageProcessor->mParams.downloadedBytes += writeBufOffset;

        /*while (writeBufOffset != kAlignmentBytes)
        {
            writeBuffer[writeBufOffset] = 0;
            writeBufOffset++;
        }*/
    // Send RPS header which is received as first chunk
    ChipLogProgress(SoftwareUpdate, "In finalize flag = %d", flag);


    count = count + writeBufOffset;
    ChipLogProgress(SoftwareUpdate, "Total size: %lu",(long unsigned int) total_size);
    ChipLogProgress(SoftwareUpdate, "Count: %lu",(long unsigned int) count);
    ChipLogProgress(SoftwareUpdate, "Write_Buff_Offset before: %lu",(long unsigned int) writeBufOffset);
    if(count >= total_size)
    {
        writeBufOffset = writeBufOffset - (count - total_size);
        last=1;
    }
    ChipLogProgress(SoftwareUpdate, "payload size kAlignmentBytes: %lu",(long unsigned int) kAlignmentBytes);
    ChipLogProgress(SoftwareUpdate, "Write_Buff_Offset after: %lu",(long unsigned int) writeBufOffset);
    ChipLogProgress(SoftwareUpdate, "payload size writeBufOffset: %lu",(long unsigned int) writeBufOffset);
	status = sl_si91x_fwup_load(writeBuffer, writeBufOffset);
	ChipLogProgress(SoftwareUpdate, "Status : %d", status);

        if (status != 0) {
            ChipLogError(SoftwareUpdate, "ERROR: In HandleFinalize for last chunk rsi_fwup() error %ld", status);
            if (status == SL_STATUS_FW_UPDATE_DONE) {
                ChipLogProgress(SoftwareUpdate,"M4 Firmware update complete");
                osDelay(5000);
                // send system reset request to reset the MCU and upgrade the m4 image
                ChipLogProgress(SoftwareUpdate,"SoC Soft Reset initiated!");
                sl_si91x_soc_soft_reset();
            } else {
            ChipLogError(SoftwareUpdate, "ERROR: In HandleFinalize for last chunk rsi_fwup() error %ld", status);
            imageProcessor->mDownloader->EndDownload(CHIP_ERROR_WRITE_FAILED);
            return;
        }
    }
    }

    imageProcessor->ReleaseBlock();

    ChipLogProgress(SoftwareUpdate, "OTA image downloaded successfully in HandleFinalize");
}

void OTAImageProcessorImpl::HandleApply(intptr_t context)
{
    uint32_t err = SL_BOOTLOADER_OK;

    ChipLogProgress(SoftwareUpdate, "OTAImageProcessorImpl::HandleApply()");

    // Force KVS to store pending keys such as data from StoreCurrentUpdateInfo()
    chip::DeviceLayer::PersistedStorage::KeyValueStoreMgrImpl().ForceKeyMapSave();

    ChipLogProgress(SoftwareUpdate, "OTA image downloaded successfully in HandleApply");

  /*  CORE_CRITICAL_SECTION(err = bootloader_verifyImage(mSlotId, NULL);)
    if (err != SL_BOOTLOADER_OK)
    {
        ChipLogError(SoftwareUpdate, "ERROR: bootloader_verifyImage() error %ld", err);
        // Call the OTARequestor API to reset the state
        GetRequestorInstance()->CancelImageUpdate();

        return;
    }

    CORE_CRITICAL_SECTION(err = bootloader_setImageToBootload(mSlotId);)
    if (err != SL_BOOTLOADER_OK)
    {
        ChipLogError(SoftwareUpdate, "ERROR: bootloader_setImageToBootload() error %ld", err);
        // Call the OTARequestor API to reset the state
        GetRequestorInstance()->CancelImageUpdate();

        return;
    }

    // This reboots the device
    CORE_CRITICAL_SECTION(bootloader_rebootAndInstall();) */
}

void OTAImageProcessorImpl::HandleAbort(intptr_t context)
{
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        return;
    }

    // Not clearing the image storage area as it is done during each write
    imageProcessor->ReleaseBlock();
}

void OTAImageProcessorImpl::HandleProcessBlock(intptr_t context)
{
    uint32_t err          = SL_BOOTLOADER_OK;
    int32_t status = 0;
    int32_t content_block = 0;
    auto * imageProcessor = reinterpret_cast<OTAImageProcessorImpl *>(context);
    if (imageProcessor == nullptr)
    {
        ChipLogError(SoftwareUpdate, "ImageProcessor context is null");
        return;
    }
    else if (imageProcessor->mDownloader == nullptr)
    {
        ChipLogError(SoftwareUpdate, "mDownloader is null");
        return;
    }

    ByteSpan block        = imageProcessor->mBlock;
    CHIP_ERROR chip_error = imageProcessor->ProcessHeader(block);

    if (chip_error != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Matter image header parser error %s", chip::ErrorStr(chip_error));
        imageProcessor->mDownloader->EndDownload(CHIP_ERROR_INVALID_FILE_IDENTIFIER);
        return;
    }

    // Copy data into the word-aligned writeBuffer, once it fills write its contents to the bootloader storage
    // Final data block is handled in HandleFinalize().
    uint32_t blockReadOffset = 0;

    while (blockReadOffset < block.size())
    {
        writeBuffer[writeBufOffset] = *((block.data()) + blockReadOffset);
        writeBufOffset++;
        blockReadOffset++;
        if (writeBufOffset == kAlignmentBytes)
        {
            writeBufOffset = 0;
            if(flag == OTA_HEADER)
            {
#ifdef M4_OTAF_DF
                ChipLogProgress(SoftwareUpdate, "M4_OTAF_DF enabled");
#endif
#ifdef CHIP_917
                ChipLogProgress(SoftwareUpdate, "CHIP_917 enabled");
#endif
            flag = RPS_HEADER;
            }
            else if(flag == RPS_HEADER)
            {
                memcpy(temp_buf,writeBuffer,64);
                // Send RPS header which is received as first chunk
                ChipLogProgress(SoftwareUpdate, "In start flag = %d", flag);
                ChipLogProgress(SoftwareUpdate, "payload size kAlignmentBytes: %lu",(long unsigned int) kAlignmentBytes);
                status = sl_si91x_fwup_start(writeBuffer);

                ChipLogProgress(SoftwareUpdate, "In rps header status: 0x%lX", status);

                // Send RPS content
                ChipLogProgress(SoftwareUpdate,"In load flag= %d\r\n", flag);
                ChipLogProgress(SoftwareUpdate,"payload size kAlignmentBytes: %lu\r\n",(long unsigned int) kAlignmentBytes);
                for(int m=0; m<64; m=m+4)
        {
ChipLogProgress(SoftwareUpdate, "data: 0x%x 0x%x 0x%x 0x%x", (long unsigned int) writeBuffer[m], (long unsigned int) writeBuffer[m+1], (long unsigned int) writeBuffer[m+2], (long unsigned int) writeBuffer[m+3]);
}
            ChipLogProgress(SoftwareUpdate," *********  temp_buf *********** ");
           for(int m=0; m<64; m=m+4)
        {
ChipLogProgress(SoftwareUpdate, "data: 0x%x 0x%x 0x%x 0x%x", (long unsigned int) temp_buf[m], (long unsigned int) temp_buf[m+1], (long unsigned int) temp_buf[m+2], (long unsigned int) temp_buf[m+3]);
}
                status = sl_si91x_fwup_load(temp_buf, 64);
                ChipLogProgress(SoftwareUpdate,"In flag 1 case statusssss: 0x%lX", status);

                ChipLogProgress(SoftwareUpdate,"In flag 1 case statusssss: 0x%lX", status);
                total_size = writeBuffer[8]+256*writeBuffer[9]+256*256*writeBuffer[10]+256*256*256*writeBuffer[11] + 64;
                ChipLogProgress(SoftwareUpdate,"Total size: %lu\r\n",(long unsigned int) total_size);

                count = count + kAlignmentBytes;
                flag = RPS_DATA;
            }
            else if(flag == RPS_DATA)
            {
                // Send RPS content
                count = count + kAlignmentBytes;
                if(count >= total_size)
                {
                    length = kAlignmentBytes - (count - total_size);
                    last = 1;
                }
                else
                {
	                length = kAlignmentBytes;
                }
                status = sl_si91x_fwup_load(writeBuffer, length);
                ChipLogProgress(SoftwareUpdate, "status: 0x%lX", status);
	            if (status != 0) {
					ChipLogProgress(SoftwareUpdate, "status: 0x%lX", status);
                    if (status == SL_STATUS_FW_UPDATE_DONE) {
                        ChipLogProgress(SoftwareUpdate, "M4 Firmware update complete");
                        // send system reset request to reset the MCU and upgrade the m4 image
                        osDelay(5000);
                        sl_si91x_soc_soft_reset();
                    }
                    else
                    {
                         ChipLogError(SoftwareUpdate, "ERROR: In HandleProcessBlock for middle chunk sl_si91x_fwup_load() error %ld", status);
                         imageProcessor->mDownloader->EndDownload(CHIP_ERROR_WRITE_FAILED);
                         return;
                    }
                }
            }
         /*  if(flag < 3)
           {
              flag++;
           }*/
           mWriteOffset += kAlignmentBytes;
           imageProcessor->mParams.downloadedBytes += kAlignmentBytes;
         }
     }
     imageProcessor->mDownloader->FetchNextData();
}

CHIP_ERROR OTAImageProcessorImpl::ProcessHeader(ByteSpan & block)
{
    if (mHeaderParser.IsInitialized())
    {
        OTAImageHeader header;
        CHIP_ERROR error = mHeaderParser.AccumulateAndDecode(block, header);

        // Needs more data to decode the header
        ReturnErrorCodeIf(error == CHIP_ERROR_BUFFER_TOO_SMALL, CHIP_NO_ERROR);
        ReturnErrorOnFailure(error);

        // SL TODO -- store version somewhere
        ChipLogProgress(SoftwareUpdate, "Image Header software version: %ld payload size: %lu", header.mSoftwareVersion,
                        (long unsigned int) header.mPayloadSize);
        mParams.totalFileBytes = header.mPayloadSize;
        mHeaderParser.Clear();
    }
    return CHIP_NO_ERROR;
}

// Store block data for HandleProcessBlock to access
CHIP_ERROR OTAImageProcessorImpl::SetBlock(ByteSpan & block)
{
    if ((block.data() == nullptr) || block.empty())
    {
        return CHIP_NO_ERROR;
    }

    // Allocate memory for block data if we don't have enough already
    if (mBlock.size() < block.size())
    {
        ReleaseBlock();

        mBlock = MutableByteSpan(static_cast<uint8_t *>(chip::Platform::MemoryAlloc(block.size())), block.size());
        if (mBlock.data() == nullptr)
        {
            return CHIP_ERROR_NO_MEMORY;
        }
    }

    // Store the actual block data
    CHIP_ERROR err = CopySpanToMutableSpan(block, mBlock);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(SoftwareUpdate, "Cannot copy block data: %" CHIP_ERROR_FORMAT, err.Format());
        return err;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR OTAImageProcessorImpl::ReleaseBlock()
{
    if (mBlock.data() != nullptr)
    {
        chip::Platform::MemoryFree(mBlock.data());
    }

    mBlock = MutableByteSpan();
    return CHIP_NO_ERROR;
}

} // namespace chip
