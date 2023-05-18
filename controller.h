#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "contentprocessor.h"

#include <QThread>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList topWords READ topWords NOTIFY topWordsChanged)
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    QVariantList topWords() const;
    double progress();

signals:
    void topWordsChanged();
    void progressChanged();
    void buttonClicked();

private:
    QThread m_fileLoaderThread;
    QThread m_contentProcessorThread;
    ContentProcessor* m_contentProcessor;

};
Q_DECLARE_METATYPE(Controller*)

#endif // CONTROLLER_H
