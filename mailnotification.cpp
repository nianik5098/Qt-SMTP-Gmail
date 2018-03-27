#include "mailnotification.h"

char* MailNotification::payload_text = NULL;

MailNotification::MailNotification(string from, string to, string cc, string subject, string message)
{
    this->from = from;
    this->to = to;
    this->cc = cc;
    this->subject = subject;
    this->message = message;
    this->curl = curl_easy_init();
}

MailNotification::MailNotification()
{
    this->curl = curl_easy_init();
}

MailNotification::~MailNotification()
{
    //curl_easy_cleanup(this->curl);
}

void MailNotification::resetParameters()
{
    this->res = CURLE_OK;
    this->recipients = NULL;
    this->upload_ctx.lines_read = 0;
    qDebug() << "init curl done";
}

void MailNotification::setDate()
{
    time_t now = time(0);
    this->date = ctime(&now);
    qDebug() << "date";
}

size_t MailNotification::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    char *data;
    char *tt;

    if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }

    data = strchr(payload_text, '\n');
    if(data)
    {
        size_t pSize = strlen(payload_text);
        size_t dSize = strlen(data);
        size_t diff = pSize-dSize;
        //qDebug() << payload_text;
        tt = (char*)malloc((diff + 1) * sizeof(char));
        strncpy(tt, payload_text, diff+1);

        tt[diff+1] = '\0';

        //qDebug() << diff;

        //qDebug() << tt;

        if(tt) {
            size_t len = strlen(tt);
            memcpy(ptr, tt, len);
            upload_ctx->lines_read++;
            qDebug() << len;
            payload_text = data + 1;
            //free(tt);
            return len;
        }
    }

    qDebug() << "000";
    return 0;
}

void MailNotification::setUser(string username, string password)
{
    this->from = this->username = username;
    this->password = password;
    qDebug() << "u-pass";
}

void MailNotification::setAllCurlOptions()
{
    curl_easy_setopt(this->curl, CURLOPT_USERNAME, this->username.c_str());
    curl_easy_setopt(this->curl, CURLOPT_PASSWORD, this->password.c_str());

    curl_easy_setopt(this->curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");

    curl_easy_setopt(this->curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    curl_easy_setopt(this->curl, CURLOPT_MAIL_FROM, this->from.c_str());

    this->recipients = curl_slist_append(this->recipients, this->to.c_str());
    if(this->cc != ""){
        this->recipients = curl_slist_append(this->recipients, this->cc.c_str());
    }
    curl_easy_setopt(this->curl, CURLOPT_MAIL_RCPT, this->recipients);

    curl_easy_setopt(this->curl, CURLOPT_READFUNCTION, this->payload_source);
    curl_easy_setopt(this->curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(this->curl, CURLOPT_UPLOAD, 1L);

    //curl_easy_setopt(this->curl, CURLOPT_VERBOSE, 1L);
}

void MailNotification::sendEmail()
{
    if(this->from != this->username){
        qDebug() << "the \"from\" account and the \"username\" doesn't match";
        return;
    }

    if(this->to == "" || this->from == "" || this->subject == "" || this->message == ""){
        qDebug() << "Either From, To, Subject or Message missing";
        return;
    }

    resetParameters();
    setDate();
    setPayloadText();
    setAllCurlOptions();

    this->res = curl_easy_perform(this->curl);

    /* Check for errors */
    if(this->res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(this->res));

    /* Free the list of recipients */
    qDebug() << "before free recipients";
    curl_slist_free_all(this->recipients);
    qDebug() << "after free recipients";
}

void MailNotification::changeFrom(string f)
{
    this->from = f;
}

void MailNotification::changeTo(string t)
{
    this->to = t;
}

void MailNotification::changeCc(string c)
{
    this->cc = c;
}

void MailNotification::changeSubject(string s)
{
    this->subject = s;
}

void MailNotification::changeMessage(string m)
{
    this->message = m;
}

void MailNotification::setPayloadText()
{
    payload_text = NULL;
    string to_str = "To: <" +this->to+ ">\r\n";
    string from_str = "From: <" +this->from+ ">\r\n";
    string cc_str = "Cc: <" + this->cc + ">\r\n";
    string date_str = "Date: " + this->date;
    string subject_str = "Subject: " + this->subject +"\r\n";
    string message_str = this->message + "\r\n";
    qDebug() << "message";

    string xxx;

    if(this->cc == ""){
        xxx = date_str + to_str + from_str + string("Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9668efd@rfcpedant.example.org>\r\n") + subject_str + string("\r\n") + message_str;
    }else{
        xxx = date_str + to_str + from_str + cc_str + string("Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9668efd@rfcpedant.example.org>\r\n") + subject_str + string("\r\n") + message_str;

    }
    payload_text = (char*)malloc(xxx.length()*sizeof(char*));
    strcpy(payload_text ,xxx.c_str());
    qDebug() << payload_text;
    //return payload_text;

}
