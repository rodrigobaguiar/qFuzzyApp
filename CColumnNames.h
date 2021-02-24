#ifndef CCOLUMNNAMES_H
#define CCOLUMNNAMES_H

#include "CItemNames.h"

class CColumnNames : public CItemNames
{
public:
    CColumnNames();



public slots:
    void on_pbAdd_clicked() override;
};

#endif // CCOLUMNNAMES_H
