#include "util.h"
#include <curl/curl.h>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>

const char *NOTIFICATION_ENDPOINT_URL = "https://ntfy.sh/aarya-test-123456";

const char *DAY_NAMES[7] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                            "Thursday", "Friday", "Saturday"};

const char *DAY_SHORTNAMES[7] = {"Sun", "Mon", "Tue", "Wed",
                                 "Thu", "Fri", "Sat"};

const char *MONTH_NAMES[12] = {"January",   "February", "March",    "April",
                               "May",       "June",     "July",     "August",
                               "September", "October",  "November", "December"};
const int DAYS_NORMAL[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int DAYS_LEAP[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int is_leap(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_month_days(int year, int monthz) {
  return is_leap(year) ? DAYS_LEAP[monthz] : DAYS_NORMAL[monthz];
}

void debug_print_date(const char *msg, struct tm &t) {
  log_printf("%s: %04d-%02d-%02d %02d:%02d:%02d", msg, t.tm_year + 1900,
             t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}

bool date_equals(struct tm t1, struct tm t2) {
  return t1.tm_year == t2.tm_year && t1.tm_mon == t2.tm_mon &&
         t1.tm_mday == t2.tm_mday;
}

bool push_notification(const std::string &message) {
  CURL *curl = curl_easy_init();
  if (!curl)
    return false;

  // Set the URL
  curl_easy_setopt(curl, CURLOPT_URL, NOTIFICATION_ENDPOINT_URL);

  // Set the POST data
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());

  // Set a timeout (in seconds) so the app doesn't hang if network is down
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

  // Perform the request
  CURLcode res = curl_easy_perform(curl);

  // Check if the request was successful
  bool success = (res == CURLE_OK);

  if (success) {
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    // ntfy returns 200 OK on success
    success = (response_code == 200);
  } else {
    log_printf("CURL Error: %s", curl_easy_strerror(res));
  }

  // Cleanup
  curl_easy_cleanup(curl);
  return success;
}
