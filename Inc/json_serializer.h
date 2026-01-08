/*
 * json_serializer.h
 *
 *  Created on: Jan 6, 2026
 *      Author: shriya
 */

#ifndef JSON_SERIALIZER_H_
#define JSON_SERIALIZER_H_

#include <stdint.h>

#define MAX_DATA_POINTS 8
#define MAX_STRING_LEN 32

#define MAX_DEVICES 4


typedef enum
{
	SERIALIZER_OK = 0,
	SERIALIZER_BUFFER_SMALL = 1,
	SERIALIZER_NULLPTR = 2
} SerializerStatus;

typedef struct
{
	char timestamp[20];
	char meter_datetime[20];
	float total_m3;
	char status[8];
} DataPoint;

typedef struct
{
	char media[MAX_STRING_LEN];
	char meter[MAX_STRING_LEN];
	char deviceId[MAX_STRING_LEN];
	char unit[8];

	uint8_t data_count; //number of valid entries in data[]
	DataPoint data[MAX_DATA_POINTS];
} DeviceReading;

typedef struct
{
	uint8_t device_count;
	DeviceReading readings [MAX_DEVICES];
} Values;

typedef struct
{
	char gatewayId[MAX_STRING_LEN];
	char date[11];
	char deviceType[MAX_STRING_LEN];

	uint16_t interval_minutes;
	uint16_t total_readings;

	Values values;
} GatewayData ;

SerializerStatus serialize_gateway_data
(
	const GatewayData *data,
	char *buffer_out, //points to the first character of the buffer
	uint16_t buffer_size,
	uint16_t *written_length
);


#endif /* JSON_SERIALIZER_H_ */
