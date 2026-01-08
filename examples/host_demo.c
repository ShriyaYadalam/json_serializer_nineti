// Host demo main -> GCC build

#include <stdio.h>
#include "json_serializer.h"

int main(void)
{
    GatewayData gateway = {0};

    snprintf(gateway.gatewayId, sizeof(gateway.gatewayId), "gateway_1234");
    snprintf(gateway.date, sizeof(gateway.date), "1970-01-01");
    snprintf(gateway.deviceType, sizeof(gateway.deviceType), "stromleser");
    gateway.interval_minutes = 15;
    gateway.total_readings = 1;

    gateway.values.device_count = 1;

    DeviceReading *dev = &gateway.values.readings[0];
    snprintf(dev->media, sizeof(dev->media), "water");
    snprintf(dev->meter, sizeof(dev->meter), "waterstarm");
    snprintf(dev->deviceId, sizeof(dev->deviceId), "stromleser_50898527");
    snprintf(dev->unit, sizeof(dev->unit), "m3");

    dev->data_count = 1;

    DataPoint *dp = &dev->data[0];
    snprintf(dp->timestamp, sizeof(dp->timestamp), "1970-01-01 00:00");
    snprintf(dp->meter_datetime, sizeof(dp->meter_datetime), "1970-01-01 00:00");
    dp->total_m3 = 107.752f;
    snprintf(dp->status, sizeof(dp->status), "OK");

    char json_buffer[512];
    uint16_t json_length = 0;

    SerializerStatus status = serialize_gateway_data(
        &gateway,
        json_buffer,
        sizeof(json_buffer),
        &json_length
    );

    if (status != SERIALIZER_OK)
    {
        printf("Serialization failed\n");
        return 1;
    }

    printf("%s\n", json_buffer);
    return 0;
}
