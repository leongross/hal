//  MIT License
//
//  Copyright (c) 2019 Ruhr University Bochum, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2021 Max Planck Institute for Security and Privacy. All Rights reserved.
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

#include "hal_core/defines.h"
#include "module_select_model.h"
#include <QDialog>
#include <QAction>
#include <QKeySequence>
#include <QSet>
#include <QLabel>

class QDialogButtonBox;
class QTreeView;
class QTabWidget;
class QPushButton;
class QLabel;

namespace hal {

    class ModuleProxyModel;
    class Searchbar;

    /**
     * @brief The CustomModuleDialog class opens a popup window for module selection.
     *
     * This feature is mainly used to select the target for the 'move to module...' action.
     * Dialog returns 'accepted' upon any selection or when pushing 'Create new module'
     * Dialog returns 'rejected' upon Cancel button and 'Pick from graph' button
     */
    class CustomModuleDialog : public QDialog
    {
        Q_OBJECT
    public:
        /**
         * The constructor.
         *
         * @param parent - The dialog's parent.
         */
        CustomModuleDialog(QSet<u32> exclude_ids, QWidget* parent=nullptr);

        /**
         * Get the through this dialog selected id.
         *
         * @return The selected id.
         */
        u32 selectedId() const { return mSelectedId; }


    private Q_SLOTS:
        void handleTreeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
        void handleTreeDoubleClick(const QModelIndex& index);
        void handleCurrentTabChanged(int index);


    public Q_SLOTS:
        /**
         *
         * Checks if the selected module id is valid (not excluded). If yes and the doubleClick
         * parameter is set to True, the dialog is accepted. If doubleClick is False, the dialog
         * buttons will be enabled.
         *
         * @param id - The selected id.
         * @param doubleClick - True to accept the dialog when id is valid. False to enable buttons.
         */
        void handleTableSelection(u32 id, bool doubleClick);

        /**
         * Updates the module select history and accepts the dialog.
         */
        void accept() override;

        /**
         * Sets the keybind to toggle the searchbar. Used when the settings changes.
         *
         * @param seq - The new sequence.
         */
        void keybindToggleSearchbar(const QKeySequence& seq);

    private:
        u32 mSelectedId;
        QDialogButtonBox* mButtonBox;
        ModuleSelectView* mTableView;
        QTreeView* mTreeView;
        QTabWidget* mTabWidget;

        ModuleProxyModel* mModuleTreeProxyModel;
        ModuleSelectProxy* mModuleTableProxyModel;
        ModuleSelectModel* mModuleSelectModel;

        Searchbar* mSearchbar;
        QAction* mToggleSearchbar;
        QSet<u32> mExcludeIds;

        QLabel* mNoAvailable;

        void enableButtons();
        u32 treeModuleId(const QModelIndex& index);

    };
}
