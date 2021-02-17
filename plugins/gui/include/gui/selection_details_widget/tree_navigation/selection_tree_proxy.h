//  MIT License
//
//  Copyright (c) 2019 Ruhr-University Bochum, Germany, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2019 Marc Fyrbiak, Sebastian Wallat, Max Hoffmann ("ORIGINAL AUTHORS"). All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "gui/gui_utils/sort.h"

#include <QSortFilterProxyModel>
#include <QRegularExpression>

namespace hal
{
    class SelectionTreeProxyModel : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        SelectionTreeProxyModel(QObject* parent = 0);
        void applyFilterOnGraphics();
        bool isGraphicsBusy() const { return mGraphicsBusy > 0; }

        gui_utility::mSortMechanism sortMechanism();
        void setSortMechanism(gui_utility::mSortMechanism sortMechanism);

    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
        bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

    public Q_SLOTS:
        void handleFilterTextChanged(const QString& filter_text);

    private Q_SLOTS:
        void handleGlobalSettingChanged(void* sender, const QString& key, const QVariant& value);

    private:
        gui_utility::mSortMechanism mSortMechanism;
        QRegularExpression mFilterExpression;
        int mGraphicsBusy;
    };
}
