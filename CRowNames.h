#ifndef CROWNAMES_H
#define CROWNAMES_H

#include "CItemNames.h"

class CRowNames : public CItemNames
{
public:
    CRowNames();

public slots:
    void on_pbAdd_clicked() override;
};

#endif // CROWNAMES_H
