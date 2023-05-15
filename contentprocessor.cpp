#include "contentprocessor.h"

#include <QTextStream>
#include <QDebug>

ContentProcessor::ContentProcessor(QObject *parent)
    : QObject{parent}
{

}

void ContentProcessor::process(const QByteArray &content)
{
    qDebug() << "contentProcessor started";
    QString c{content};
    QHash<QString, int> topWords;
    QTextStream textStream(&c, QIODevice::ReadOnly);
    while (!textStream.atEnd()) {
        const auto words = textStream.readLine().split(' ');
        for (const QString &word : words) {
            topWords[word]++;
        }
    }

    QVariantList l;
    foreach (QString word, topWords.keys()) {
        l.append(QVariantMap({{"word", word}, {"count", topWords.value(word)}}));
    }

    std::sort(l.begin(), l.end(), [](const QVariant& lhs, const QVariant& rhs) {
        return lhs.toMap().value("count") > rhs.toMap().value("count");
    });

    // todo rewrite
    QVariantList l2;
    int i = 0;
    for (const auto word : l) {
        l2.append(word);
        if (i++ >= 15) {
            break;
        }
    }
    qDebug() << "contentProcessor ended";

    setTopWords(l2);
}

QVariantList ContentProcessor::getTopWords() const
{
    return m_topWords;
}

void ContentProcessor::setTopWords(const QVariantList &topWords)
{
    if (m_topWords == topWords) {
        return;
    }
    m_topWords = topWords;
    emit topWordsChanged();
}
