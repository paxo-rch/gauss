#include "notification.hpp"


Notification::Notification(String name, String message, String logo)
{
    STANDBY_MODE::mode=STANDBY_MODE_ASK_DISABLE;
    this->name = name;
    this->message = message;
    this->logo = logo;
    Serial.println("Notification: " + String(name) + ": " + String(message));
}

void Notification::show()
{
    showed=true;
    Box box(10, 30, 300, 100);
    box.setBackgroundColor(0xFFFF);
    box.setRadius(10);
    box.setBorderSize(2);

    Image* image = new Image(logo, 15,15);
    image->setRadius(35);
    image->load();
    box.addChild(image);

    Label* title = new Label(105, 15, 190, 24);
    title->setText(name);
    title->setBorderSize(0);
    title->bold=true;
    title->fontSize=20;
    box.addChild(title);

    Label* msg = new Label(105, 41, 190, 40);
    msg->setText(message);
    msg->setBorderSize(0);
    msg->fontSize=15;
    box.addChild(msg);

    box.drawAll();

    unsigned int timer = millis();

    while(millis() - timer > 3000 && !touch.isTouch());

    box.removechildren();
}

void NotificationManager::add(Notification * newNotification)
{
    notifications.push_back(newNotification);
}

void NotificationManager::clear(int index)
{
    if(notifications.size() == 0)
        return;
    delete notifications[index];
    notifications.erase(notifications.begin()+index);
}

Notification* NotificationManager::get(int index)
{
    return notifications[index];
}

int NotificationManager::getNumber()
{
    return notifications.size();
}

void NotificationManager::show()
{
    for(int i = 0; i < notifications.size(); i++)
    {
        if(notifications[i]->showed)
        {
            //notifications.erase(notifications.begin()+i);
            //i=0;
        }
        else
        {
            notifications[i]->show();
            //notifications.erase(notifications.begin()+i);
            //i=0;
        }
    }
}