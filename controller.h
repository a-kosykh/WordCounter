#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "contentprocessor.h"

#include <QThread>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList topWords READ topWords NOTIFY topWordsChanged)
    Q_PROPERTY(int debugNumber MEMBER m_debugNumber)
public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    QVariantList topWords() const;
    void debugPrint();

signals:
    void topWordsChanged();

private:
    QThread m_fileCheckerThread;
    QThread m_contentProcessorThread;
    ContentProcessor* m_contentProcessor;
    int m_debugNumber = 4;

};
Q_DECLARE_METATYPE(Controller*)

#endif // CONTROLLER_H