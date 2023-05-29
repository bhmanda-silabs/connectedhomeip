set -e
mkdir -p /gsdk/extension
ln -sf $(pwd)/output/wiseconnect3/ /gsdk/extension/wiseconnect3
slc signature trust -extpath /gsdk/extension/wiseconnect3
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_central/ble_central.slcp --force --with "brd4180a" -np -d output/ble_central-brd4180a
make -C output/ble_central-brd4180a -f ble_central.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_central/ble_central.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_central-brd4325a
make -C output/ble_central-brd4325a -f ble_central.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/ble/ble_chat/ble_chat.slcp --force --with "brd4180a" -np -d output/ble_chat-brd4180a
make -C output/ble_chat-brd4180a -f ble_chat.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/ble/ble_chat/ble_chat.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_chat-brd4325a
make -C output/ble_chat-brd4325a -f ble_chat.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_datalength/ble_datalength.slcp --force --with "brd4180a" -np -d output/ble_datalength-brd4180a
make -C output/ble_datalength-brd4180a -f ble_datalength.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_datalength/ble_datalength.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_datalength-brd4325a
make -C output/ble_datalength-brd4325a -f ble_datalength.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_heart_rate_profile/ble_heart_rate_profile.slcp --force --with "brd4180a" -np -d output/ble_heart_rate_profile-brd4180a
make -C output/ble_heart_rate_profile-brd4180a -f ble_heart_rate_profile.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_heart_rate_profile/ble_heart_rate_profile.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_heart_rate_profile-brd4325a
make -C output/ble_heart_rate_profile-brd4325a -f ble_heart_rate_profile.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_hid_on_gatt/ble_hid_on_gatt.slcp --force --with "brd4180a" -np -d output/ble_hid_on_gatt-brd4180a
make -C output/ble_hid_on_gatt-brd4180a -f ble_hid_on_gatt.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_hid_on_gatt/ble_hid_on_gatt.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_hid_on_gatt-brd4325a
make -C output/ble_hid_on_gatt-brd4325a -f ble_hid_on_gatt.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_ibeacon/ble_ibeacon.slcp --force --with "brd4180a" -np -d output/ble_ibeacon-brd4180a
make -C output/ble_ibeacon-brd4180a -f ble_ibeacon.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_ibeacon/ble_ibeacon.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_ibeacon-brd4325a
make -C output/ble_ibeacon-brd4325a -f ble_ibeacon.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_longrange_2mbps/ble_longrange_2mbps.slcp --force --with "brd4180a" -np -d output/ble_longrange_2mbps-brd4180a
make -C output/ble_longrange_2mbps-brd4180a -f ble_longrange_2mbps.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_longrange_2mbps/ble_longrange_2mbps.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_longrange_2mbps-brd4325a
make -C output/ble_longrange_2mbps-brd4325a -f ble_longrange_2mbps.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/ble_per/ble_per.slcp --force --with "brd4180a" -np -d output/ble_per-brd4180a
make -C output/ble_per-brd4180a -f ble_per.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/ble_per/ble_per.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_per-brd4325a
make -C output/ble_per-brd4325a -f ble_per.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/ble/ble_peripheral/ble_peripheral.slcp --force --with "brd4180a" -np -d output/ble_peripheral-brd4180a
make -C output/ble_peripheral-brd4180a -f ble_peripheral.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/ble/ble_peripheral/ble_peripheral.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_peripheral-brd4325a
make -C output/ble_peripheral-brd4325a -f ble_peripheral.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_power_save/ble_power_save.slcp --force --with "brd4180a" -np -d output/ble_power_save-brd4180a
make -C output/ble_power_save-brd4180a -f ble_power_save.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_power_save/ble_power_save.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_power_save-brd4325a
make -C output/ble_power_save-brd4325a -f ble_power_save.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_privacy/ble_privacy.slcp --force --with "brd4180a" -np -d output/ble_privacy-brd4180a
make -C output/ble_privacy-brd4180a -f ble_privacy.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_privacy/ble_privacy.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_privacy-brd4325a
make -C output/ble_privacy-brd4325a -f ble_privacy.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_secureconnection/ble_secureconnection.slcp --force --with "brd4180a" -np -d output/ble_secureconnection-brd4180a
make -C output/ble_secureconnection-brd4180a -f ble_secureconnection.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_secureconnection/ble_secureconnection.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_secureconnection-brd4325a
make -C output/ble_secureconnection-brd4325a -f ble_secureconnection.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/ble/ble_simple_smp/ble_simple_smp.slcp --force --with "brd4180a" -np -d output/ble_simple_smp-brd4180a
make -C output/ble_simple_smp-brd4180a -f ble_simple_smp.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/ble/ble_simple_smp/ble_simple_smp.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_simple_smp-brd4325a
make -C output/ble_simple_smp-brd4325a -f ble_simple_smp.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_throughput_app/ble_throughput_app.slcp --force --with "brd4180a" -np -d output/ble_throughput_app-brd4180a
make -C output/ble_throughput_app-brd4180a -f ble_throughput_app.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_throughput_app/ble_throughput_app.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_throughput_app-brd4325a
make -C output/ble_throughput_app-brd4325a -f ble_throughput_app.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_white_list/ble_white_list.slcp --force --with "brd4180a" -np -d output/ble_white_list-brd4180a
make -C output/ble_white_list-brd4180a -f ble_white_list.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/ble/ble_white_list/ble_white_list.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/ble_white_list-brd4325a
make -C output/ble_white_list-brd4325a -f ble_white_list.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/wifi6_mimo/wifi6_mimo.slcp --force --with "brd4180a" -np -d output/wifi6_mimo-brd4180a
make -C output/wifi6_mimo-brd4180a -f wifi6_mimo.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/wifi6_mimo/wifi6_mimo.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi6_mimo-brd4325a
make -C output/wifi6_mimo-brd4325a -f wifi6_mimo.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/access_point/access_point.slcp --force --with "brd4180a" -np -d output/wifi_access_point-brd4180a
make -C output/wifi_access_point-brd4180a -f wifi_access_point.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/access_point/access_point.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_access_point-brd4325a
make -C output/wifi_access_point-brd4325a -f wifi_access_point.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/aws_device_shadow/aws_device_shadow.slcp --force --with "brd4180a" -np -d output/wifi_aws_device_shadow-brd4180a
make -C output/wifi_aws_device_shadow-brd4180a -f wifi_aws_device_shadow.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/aws_device_shadow/aws_device_shadow.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_aws_device_shadow-brd4325a
make -C output/wifi_aws_device_shadow-brd4325a -f wifi_aws_device_shadow.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/cloud_apps/aws/mqtt/aws_mqtt.slcp --force --with "brd4180a" -np -d output/wifi_aws_mqtt-brd4180a
make -C output/wifi_aws_mqtt-brd4180a -f wifi_aws_mqtt.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/cloud_apps/aws/mqtt/aws_mqtt.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_aws_mqtt-brd4325a
make -C output/wifi_aws_mqtt-brd4325a -f wifi_aws_mqtt.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/wlan_ble/wifi_ble_power_save/wifi_ble_power_save.slcp --force --with "brd4180a" -np -d output/wifi_ble_power_save-brd4180a
make -C output/wifi_ble_power_save-brd4180a -f wifi_ble_power_save.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/wlan_ble/wifi_ble_power_save/wifi_ble_power_save.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_ble_power_save-brd4325a
make -C output/wifi_ble_power_save-brd4325a -f wifi_ble_power_save.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/concurrent_mode/concurrent_mode.slcp --force --with "brd4180a" -np -d output/wifi_concurrent_mode-brd4180a
make -C output/wifi_concurrent_mode-brd4180a -f wifi_concurrent_mode.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/concurrent_mode/concurrent_mode.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_concurrent_mode-brd4325a
make -C output/wifi_concurrent_mode-brd4325a -f wifi_concurrent_mode.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/embedded_mqtt_client/embedded_mqtt_client.slcp --force --with "brd4180a" -np -d output/wifi_embedded_mqtt_client-brd4180a
make -C output/wifi_embedded_mqtt_client-brd4180a -f wifi_embedded_mqtt_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/embedded_mqtt_client/embedded_mqtt_client.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_embedded_mqtt_client-brd4325a
make -C output/wifi_embedded_mqtt_client-brd4325a -f wifi_embedded_mqtt_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/embedded_mqtt_client_twt/embedded_mqtt_client_twt.slcp --force --with "brd4180a" -np -d output/wifi_embedded_mqtt_client_twt-brd4180a
make -C output/wifi_embedded_mqtt_client_twt-brd4180a -f wifi_embedded_mqtt_client_twt.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/embedded_mqtt_client_twt/embedded_mqtt_client_twt.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_embedded_mqtt_client_twt-brd4325a
make -C output/wifi_embedded_mqtt_client_twt-brd4325a -f wifi_embedded_mqtt_client_twt.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/enterprise_client/enterprise_client.slcp --force --with "brd4180a" -np -d output/wifi_enterprise_client-brd4180a
make -C output/wifi_enterprise_client-brd4180a -f wifi_enterprise_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/enterprise_client/enterprise_client.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_enterprise_client-brd4325a
make -C output/wifi_enterprise_client-brd4325a -f wifi_enterprise_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/firmware_update/firmware_update.slcp --force --with "brd4180a" -np -d output/wifi_firmware_update-brd4180a
make -C output/wifi_firmware_update-brd4180a -f wifi_firmware_update.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/firmware_update/firmware_update.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_firmware_update-brd4325a
make -C output/wifi_firmware_update-brd4325a -f wifi_firmware_update.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/wlan/http_client/http_client_app.slcp --force --with "brd4180a" -np -d output/wifi_http_client_app-brd4180a
make -C output/wifi_http_client_app-brd4180a -f wifi_http_client_app.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/wlan/http_client/http_client_app.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_http_client_app-brd4325a
make -C output/wifi_http_client_app-brd4325a -f wifi_http_client_app.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/wlan/http_client_post_data/http_client_post_data_app.slcp --force --with "brd4180a" -np -d output/wifi_http_client_post_data-brd4180a
make -C output/wifi_http_client_post_data-brd4180a -f wifi_http_client_post_data.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/_internal/Wireless_Examples/wlan/http_client_post_data/http_client_post_data_app.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_http_client_post_data-brd4325a
make -C output/wifi_http_client_post_data-brd4325a -f wifi_http_client_post_data.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/http_otaf/http_otaf.slcp --force --with "brd4180a" -np -d output/wifi_http_otaf-brd4180a
make -C output/wifi_http_otaf-brd4180a -f wifi_http_otaf.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/http_otaf/http_otaf.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_http_otaf-brd4325a
make -C output/wifi_http_otaf-brd4325a -f wifi_http_otaf.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/power_save_deep_sleep/power_save_deep_sleep.slcp --force --with "brd4180a" -np -d output/wifi_power_save_deep_sleep-brd4180a
make -C output/wifi_power_save_deep_sleep-brd4180a -f wifi_power_save_deep_sleep.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/power_save_deep_sleep/power_save_deep_sleep.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_power_save_deep_sleep-brd4325a
make -C output/wifi_power_save_deep_sleep-brd4325a -f wifi_power_save_deep_sleep.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/powersave_standby_associated/powersave_standby_associated.slcp --force --with "brd4180a" -np -d output/wifi_powersave_standby_associated-brd4180a
make -C output/wifi_powersave_standby_associated-brd4180a -f wifi_powersave_standby_associated.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/powersave_standby_associated/powersave_standby_associated.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_powersave_standby_associated-brd4325a
make -C output/wifi_powersave_standby_associated-brd4325a -f wifi_powersave_standby_associated.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan_ble/wifi_station_ble_provisioning/wifi_station_ble_provisioning.slcp --force --with "brd4180a" -np -d output/wifi_station_ble_provisioning-brd4180a
make -C output/wifi_station_ble_provisioning-brd4180a -f wifi_station_ble_provisioning.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan_ble/wifi_station_ble_provisioning/wifi_station_ble_provisioning.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_station_ble_provisioning-brd4325a
make -C output/wifi_station_ble_provisioning-brd4325a -f wifi_station_ble_provisioning.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan_ble/wifi_station_ble_provisioning_aws/wifi_station_ble_provisioning_aws.slcp --force --with "brd4180a" -np -d output/wifi_station_ble_provisioning_aws-brd4180a
make -C output/wifi_station_ble_provisioning_aws-brd4180a -f wifi_station_ble_provisioning_aws.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan_ble/wifi_station_ble_provisioning_aws/wifi_station_ble_provisioning_aws.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_station_ble_provisioning_aws-brd4325a
make -C output/wifi_station_ble_provisioning_aws-brd4325a -f wifi_station_ble_provisioning_aws.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/tls_client/tls_client.slcp --force --with "brd4180a" -np -d output/wifi_tls_client-brd4180a
make -C output/wifi_tls_client-brd4180a -f wifi_tls_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/tls_client/tls_client.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_tls_client-brd4325a
make -C output/wifi_tls_client-brd4325a -f wifi_tls_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/twt_tcp_client/twt_tcp_client.slcp --force --with "brd4180a" -np -d output/wifi_twt_tcp_client-brd4180a
make -C output/wifi_twt_tcp_client-brd4180a -f wifi_twt_tcp_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/snippets/wlan/twt_tcp_client/twt_tcp_client.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_twt_tcp_client-brd4325a
make -C output/wifi_twt_tcp_client-brd4325a -f wifi_twt_tcp_client.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/wlan_throughput/wlan_throughput.slcp --force --with "brd4180a" -np -d output/wifi_wlan_throughput-brd4180a
make -C output/wifi_wlan_throughput-brd4180a -f wifi_wlan_throughput.Makefile -j
slc generate /gsdk/extension/wiseconnect3/examples/featured/wlan_throughput/wlan_throughput.slcp --force --with "brd4325a;wiseconnect3_sdk" -np -d output/wifi_wlan_throughput-brd4325a
make -C output/wifi_wlan_throughput-brd4325a -f wifi_wlan_throughput.Makefile -j
