/*
 * Alerts.h
 *
 *  Created on: Aug 11, 2022
 *      Author: Marcin Nadarzyński
 */

#ifndef APPLICATION_USER_CORE_ALERTS_H_
#define APPLICATION_USER_CORE_ALERTS_H_

enum alertLevel{
	Critical,
	Error,
	Warning,
	Info,
};

typedef struct
{
	alertLevel level;
	char text[100];
}alert_t;

alert_t alerts[10];
static alertNumber=0;
void addAlert(alertLevel level, char* text);
void removeAlert(alerLevel level, char* text);
void sortAlerts();

#endif /* APPLICATION_USER_CORE_ALERTS_H_ */
