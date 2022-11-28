#ifndef NOTIFICATION
#define NOTIFICATION

// don't use heritate from Gauss

class Notification
{
    public:
    Notification(String name, String message, String logo = "");
    void show();

    String getName(){ return name; }
    String getMessage(){ return message; }
    String getLogo(){ return logo; }

    bool showed = false;

    private:
    String name;
    String message;
    String logo;
};

class NotificationManager
{
    public:
    void add(Notification * newNotification);
    Notification* get(int index);
    int getNumber();
    void clear(int index);

    void show();

    private:

    protected:
    std::vector<Notification *> notifications;
};

NotificationManager notifications;

void notifications_update()
{
    notifications.show();
}

#endif