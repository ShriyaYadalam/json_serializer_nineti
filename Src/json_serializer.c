/*
 * json_serializer.c
 *
 *  Created on: Jan 6, 2026
 *      Author: shriya
 */

#include "json_serializer.h"
#include <stdio.h>
#include <inttypes.h>

SerializerStatus serialize_gateway_data
(
    const GatewayData *data,
    char *buffer_out,
    uint16_t buffer_size,
    uint16_t *written_length
)
{
    if (data == NULL || buffer_out == NULL || written_length == NULL)
    {
        return SERIALIZER_NULLPTR;
    }

    uint16_t offset = 0;
    int ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset, "[\n{\n");
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"gatewayId\": \"%s\",\n", data->gatewayId);
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"date\": \"%s\",\n", data->date);
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"deviceType\": \"%s\",\n", data->deviceType);
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"interval_minutes\": %u,\n", data->interval_minutes);
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"total_readings\": %u,\n", data->total_readings);
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"values\": {\n");
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"device_count\": %u,\n",
                   data->values.device_count);
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\"readings\": [\n");
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    for (uint8_t i = 0; i < data->values.device_count; i++)
    {
        const DeviceReading *dev = &data->values.readings[i];

        ret = snprintf(buffer_out + offset, buffer_size - offset, "{\n");
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        ret = snprintf(buffer_out + offset, buffer_size - offset,
                       "\"media\": \"%s\",\n", dev->media);
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        ret = snprintf(buffer_out + offset, buffer_size - offset,
                       "\"meter\": \"%s\",\n", dev->meter);
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        ret = snprintf(buffer_out + offset, buffer_size - offset,
                       "\"deviceId\": \"%s\",\n", dev->deviceId);
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        ret = snprintf(buffer_out + offset, buffer_size - offset,
                       "\"unit\": \"%s\",\n", dev->unit);
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        ret = snprintf(buffer_out + offset, buffer_size - offset,
                       "\"data\": [\n");
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        for (uint8_t j = 0; j < dev->data_count; j++)
        {
            const DataPoint *dp = &dev->data[j];

            ret = snprintf(buffer_out + offset, buffer_size - offset, "{\n");
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;

            ret = snprintf(buffer_out + offset, buffer_size - offset,
                           "\"timestamp\": \"%s\",\n", dp->timestamp);
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;

            ret = snprintf(buffer_out + offset, buffer_size - offset,
                           "\"meter_datetime\": \"%s\",\n", dp->meter_datetime);
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;

            uint32_t whole = (uint32_t)dp->total_m3;
            uint32_t frac  = (uint32_t)((dp->total_m3 - whole) * 1000.0f);

            ret = snprintf(buffer_out + offset, buffer_size - offset, "\"total_m3\": %" PRIu32 ".%03" PRIu32 ",\n", whole, frac);
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;

            ret = snprintf(buffer_out + offset, buffer_size - offset,
                           "\"status\": \"%s\"\n", dp->status);
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;

            ret = snprintf(buffer_out + offset, buffer_size - offset, "}");
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;

            if (j + 1 < dev->data_count)
            {
                ret = snprintf(buffer_out + offset, buffer_size - offset, ",\n");
                if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
                offset += (uint16_t)ret;
            }
        }

        ret = snprintf(buffer_out + offset, buffer_size - offset, "\n]\n");
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        ret = snprintf(buffer_out + offset, buffer_size - offset, "}");
        if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
        offset += (uint16_t)ret;

        if (i + 1 < data->values.device_count)
        {
            ret = snprintf(buffer_out + offset, buffer_size - offset, ",\n");
            if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
            offset += (uint16_t)ret;
        }
    }

    ret = snprintf(buffer_out + offset, buffer_size - offset,
                   "\n]\n}\n}\n]\n");
    if (ret < 0 || ret >= (buffer_size - offset)) return SERIALIZER_BUFFER_SMALL;
    offset += (uint16_t)ret;

    *written_length = offset;
    return SERIALIZER_OK;
}
