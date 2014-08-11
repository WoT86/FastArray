#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QObject>

class ProjectManager;

class ProjectSettings : public QObject
{
    Q_OBJECT

    friend class ProjectManager;
public:
    explicit ProjectSettings(QObject *parent = 0);

signals:

public slots:

};

#endif // PROJECTSETTINGS_H
