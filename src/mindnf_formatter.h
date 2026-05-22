#pragma once

#include <QString>

#include "mindnf.h"

QString minDNFDisplayHtml(const MinDNF& d, ll n);
QString minDNFClipboardPlain(const MinDNF& d, ll n);
QString constantDNFDisplayHtml(ll value);
QString constantDNFClipboardPlain(ll value);
