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

#include "hal_core/netlist/boolean_function.h"

namespace hal
{
    namespace BooleanFunctionParser
    {
        /// ParserType refers to the parser identifier
        enum class ParserType
        {
            /// refers to the 'Liberty' Boolean function parser
            Liberty,
            /// refers to the 'Standard' Boolean function parser
            Standard,
        };

        /// TokenType refers to a token identifier for a Boolean function string.
        enum class TokenType
        {
            /// refers to a Boolean And operation (e.g., "&")
            And,
            /// refers to a Boolean Not operation (e.g., "!")
            Not,
            /// refers to a Boolean Or operation (e.g., "|"")
            Or,
            /// refers to a Boolean Xor operation (e.g., "^")
            Xor,

            /// refers to a Boolean variable (e.g., "i0")
            Variable,
            /// refers to a Boolean constant (e.g., "0" or "1")
            Constant,

            /// refers to an open bracket (e.g., "(")
            BracketOpen,
            /// refers to a closed open bracket (e.g., "(")
            BracketClose,
        };

        /// Token refers to a token identifier and accompanied data.
        struct Token
        {
            ////////////////////////////////////////////////////////////////////////////
            // Member
            ////////////////////////////////////////////////////////////////////////////

            /// refers to the underlying token type identifier
            TokenType type;
            /// optional value and bit-size in case token is a variable
            std::tuple<std::string, u16> variable{};
            /// optional value in case token is a constant
            std::vector<BooleanFunction::Value> constant{};

            ////////////////////////////////////////////////////////////////////////////
            // Constructors, Destructors, Operators
            ////////////////////////////////////////////////////////////////////////////

            /// Constructor to initialize all Token members.
            Token(TokenType _type, std::tuple<std::string, u16> variable, std::vector<BooleanFunction::Value> constant);

            /// Creates an "AND" token.
            static Token And();
            /// Creates an "Not" token.
            static Token Not();
            /// Creates an "Or" token.
            static Token Or();
            /// Creates an "Xor" token.
            static Token Xor();

            /// Creates a variable token.
            static Token Variable(std::string name, u16 size);
            /// Creates a constant token.
            static Token Constant(std::vector<BooleanFunction::Value> value);

            /// Creates and "BracketOpen" token.
            static Token BracketOpen();
            /// Creates and "BrackentClose" token.
            static Token BracketClose();

            ////////////////////////////////////////////////////////////////////////////
            // Interface
            ////////////////////////////////////////////////////////////////////////////

            /// Returns the precedence of a token.
            ///
            /// @param[in] type Parser type identifier.
            /// @returns unsigned Predecene value in between 2..4
            unsigned precedence(const ParserType& type) const;

            /// Short-hand implementation to check for a token type.
            ///
            /// @param[in] type Token type.
            /// @returns bool - `true` in case instance has the same token type, `false` otherwise.
            bool is(TokenType type) const;

            /// Human-readable debug string representation of the token.
            friend std::ostream& operator<<(std::ostream& os, const Token& token);
            /// Human-readable debug string representation of the token.
            std::string to_string() const;
        };

        /**
         * Parses a Boolean function from a string representation into its tokens.
         * 
         * @param[in] expression Boolean function string.
         * @returns The list of tokens on success, error message string otherwise.
         */
        std::variant<std::vector<Token>, std::string> parse_with_standard_grammar(const std::string& expression);

        /**
         * Parses a Boolean function from a string representation into its tokens
         * based on the data format defined for Liberty, see Liberty user guide.
         * 
         * @param[in] expression Boolean function string.
         * @returns The list of tokens on success, error message string otherwise.
         */
        std::variant<std::vector<Token>, std::string> parse_with_liberty_grammar(const std::string& expression);

        /**
         * Transforms a list of tokens in infix notation (e.g., "A & B") into the
         * reverse polish notation notation (e.g., "A B &") that allows for an easy
         * translation into the AST data structure. 
         * 
         * In order to translate between the different notations, we leverage the
         * well-known Shunting Yard algorithm, see wikipedia for an example:
         *  https://en.wikipedia.org/wiki/Shunting-yard_algorithm
         * 
         * @param[in] tokens List of tokens in infix notation.
         * @param[in] expression Expression string.
         * @param[in] parser Parser identifier
         * @returns[out] List of tokens in reverse-polish notation on success, error message string otherwise.
         */
        std::variant<std::vector<Token>, std::string> reverse_polish_notation(std::vector<Token>&& tokens, const std::string& expression, const ParserType& parser);

        /**
         * Translates a list of tokens (in reverse-polish notation form) into a list
         * of BooleanFunction nodes that are then assembled into a Boolean function.
         * 
         * @param[in] tokens List of tokens.
         * @param[in] expression Expression string.
         * @returns The Boolean function on success, error message string otherwise.
         */
        std::variant<BooleanFunction, std::string> translate(std::vector<Token>&& tokens, const std::string& expression);

    }    // namespace BooleanFunctionParser
}    // namespace hal
