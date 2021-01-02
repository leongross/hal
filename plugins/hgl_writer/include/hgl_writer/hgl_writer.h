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

// TODO remove
#include "gate_library_specific_utils/include/plugin_gate_library_specific_utils.h"
// TODO end remove

#include "hal_core/defines.h"
#include "hal_core/netlist/boolean_function.h"
#include "hal_core/netlist/gate_library/gate_library_writer/gate_library_writer.h"
#include "hal_core/netlist/gate_library/gate_type.h"

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace hal
{
    class GateType;

    /**
     * @ingroup netlist
     */
    class NETLIST_API HGLWriter : public GateLibraryWriter
    {
    public:
        HGLWriter()  = default;
        ~HGLWriter() = default;

        /**
         * Write the gate library to a file at the provided location.
         *
         * @param[in] gate_lib - The gate library.
         * @param[in] file_path - The output path.
         * @returns True on success.
         */
        bool write(const GateLibrary* gate_lib, const std::filesystem::path& file_path) override;

    private:
        struct PinCtx
        {
            std::string name;
            std::string direction;
            std::string type       = "";
            std::string function   = "";
            std::string x_function = "";
            std::string z_function = "";
        };

        struct GroupCtx
        {
            std::string name;
            std::map<u32, std::string> index_to_pin;
        };

        // TODO remove
        std::unique_ptr<gate_library_specific_utils::Utils> m_utils;
        // TODO end remove

        static const std::unordered_map<GateType::ClearPresetBehavior, std::string> m_behavior_to_string;
        static const std::unordered_map<GateType::BaseType, std::string> m_base_type_to_string;
        static const std::unordered_map<GateType::PinType, std::string> m_pin_type_to_string;

        bool write_gate_library(rapidjson::Document& document, const GateLibrary* gate_lib);

        std::vector<PinCtx> get_pins(GateType* gt, const std::unordered_map<std::string, BooleanFunction>& functions);
        std::vector<GroupCtx> get_groups(GateType* gt);
    };
}    // namespace hal
