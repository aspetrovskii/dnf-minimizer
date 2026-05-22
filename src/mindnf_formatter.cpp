#include "mindnf_formatter.h"

namespace {

QString disjunctDisplayHtml(const Disjunct& d, ll n) {
    QString t;
    for (ll k = 0; k < n; ++k) {
        if (((d.mask() >> k) & 1) == 0) {
            continue;
        }
        const QChar letter(static_cast<char16_t>('A' + k));
        if ((d.kit() >> k) & 1) {
            t += QLatin1String("<span style=\"font-size: 22pt;\">") + QString(letter) + QLatin1String("</span>");
        } else {
            t += QLatin1String("<span style=\"font-size: 22pt;\">") + QString(letter) + QLatin1String("&#x305;</span>");
        }
    }
    return t;
}

QString disjunctClipboardPlain(const Disjunct& d, ll n) {
    QString t;
    const QChar neg(0x00AC);
    for (ll k = 0; k < n; ++k) {
        if (((d.mask() >> k) & 1) == 0) {
            continue;
        }
        const QChar letter(static_cast<char16_t>('A' + k));
        if ((d.kit() >> k) & 1) {
            t += letter;
        } else {
            t += neg;
            t += letter;
        }
    }
    return t;
}

}  // namespace

QString minDNFDisplayHtml(const MinDNF& d, ll n) {
    if (d.empty()) {
        return QString();
    }
    QString html = QLatin1String("<div style=\"font-family: Arial;\">");
    ll first = 1;
    for (const auto& [disj, _] : d.disjuncts()) {
        if (!first) {
            html += QLatin1String(" <span style=\"font-size: 28pt;\">+</span> ");
        }
        first = 0;
        html += disjunctDisplayHtml(disj, n);
    }
    html += QLatin1String("</div>");
    return html;
}

QString constantDNFDisplayHtml(ll value) {
    return QLatin1String("<div style=\"font-family: Arial;\"><span style=\"font-size: 28pt;\">") +
           QString::number(static_cast<int>(value)) + QLatin1String("</span></div>");
}

QString constantDNFClipboardPlain(ll value) {
    return QString::number(static_cast<int>(value));
}

QString minDNFClipboardPlain(const MinDNF& d, ll n) {
    QString t;
    ll first = 1;
    for (const auto& [disj, _] : d.disjuncts()) {
        if (!first) {
            t += QLatin1String(" + ");
        }
        first = 0;
        t += disjunctClipboardPlain(disj, n);
    }
    return t;
}
