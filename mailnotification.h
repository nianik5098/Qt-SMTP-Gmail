#ifndef MAILNOTIFICATION_H
#define MAILNOTIFICATION_H

#include <QDebug>
#include <curl/curl.h>
#include <iostream>
#include <ctime>
#include <string>
#include <string.h>
#include <cstddef>

using namespace std;

struct upload_status {
    int lines_read;
};


class MailNotification
{
public:
    MailNotification(string from, string to, string cc, string subject, string message);
    MailNotification();
    ~MailNotification();
    string from;
    string to;
    string cc;
    string subject;
    string message;
    string date;

    struct upload_status upload_ctx;

    CURL *curl;
    CURLcode res;
    struct curl_slist *recipients;

    void resetParameters();
    void setPayloadText();
    void setDate();
    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
    void setUser(string username, string password);
    void setAllCurlOptions();
    void sendEmail();


    void changeFrom(string f);
    void changeTo(string t);
    void changeCc(string c);
    void changeSubject(string s);
    void changeMessage(string m);

private:
    string username;
    string password;
    static char *payload_text;
};


#endif // MAILNOTIFICATION_H
