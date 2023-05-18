#ifndef CONTENTPROCESSOR_H
#define CONTENTPROCESSOR_H

#include <QColor>
#include <QVariantList>
#include <QMap>
#include <QObject>
#include <QMutex>

class ContentProcessor : public QObject
{
    Q_OBJECT
    struct WordProperties{
        int count;
        QColor color;
    };
public:
    explicit ContentProcessor(QObject *parent = nullptr);
    void process(const QString& line, quint64 processedBytes, quint64 totalBytes);
    QVariantList getTopWords();
    double getProgress();

signals:
    void topWordsChanged();
    void progressChanged();

private:
    QMutex m_mutex;
    double m_progress;
    QHash<QString, WordProperties> m_wordsCount;
    QVariantList m_topWords;

private:
    void setTopWords(const QVariantList& topWords);
    void setProgress(double progress);
};

#endif // CONTENTPROCESSOR_H
