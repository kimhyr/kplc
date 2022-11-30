#ifndef KPLC_COMPILER_ANALYZER_LEXER_HPP
#define KPLC_COMPILER_ANALYZER_LEXER_HPP

#include "../../Text/Char.hpp"
#include "../../Collections/Dynar.hpp"
#include "../../Definitions.hpp"
#include "../Error.hpp"
#include "Token.hpp"

namespace Compiler::Analyzer {
    using namespace Compiler;
    using namespace Text;
    using namespace Collections;

    class Lexer {
    public:
        enum Flag : Bit8 {
            None = 0,
            End = 1 << 0,
            Continue = 1 << 1,
        };

        enum class ErrorCode : Error::Code {
            WrongFormat,
            Valueless,
            Incomplete,
            Conversion,
        };

    public:
        constexpr
        Lexer(const Char8 *source)
                : flags(Lexer::Flag::None),
                  point({1, 1}),
                  index(-1),
                  source(source),
                  peek(source[0]),
                  token() {}

        Void Destroy();

    public:
        constexpr
        Bit8 GetFlags()
        const noexcept { return this->flags; }

    public:
        Token Lex();

    private:
        Bit8 flags;
        Token::Point point;
        Int64 index;
        const Char8 *source;
        Char8 peek;
        Token token;

    private:
        inline
        Void LexAlphabetic()
        noexcept;

        inline
        Void MatchAlphabetic(const Char8 *flush)
        noexcept;

        inline
        Void LexNumeric();

        inline
        Void LexBinary(Dynar<Char8> *buf);

        constexpr
        Bool PeekIsValidBinary()
        const noexcept { return this->peek == '0' || this->peek == '1' || this->peek == '_'; }

        inline
        Void LexHexadecimal(Dynar<Char8> *buf);

        constexpr
        Bool PeekIsValidHexadecimal()
        const noexcept {
            return Char::IsNumeric(this->peek) || (this->peek >= 'a' && this->peek <= 'f') ||
                   (this->peek >= 'A' && this->peek <= 'F') || this->peek == '_';
        }

        inline
        Void LexReal(Dynar<Char8> *buf);

        inline
        Void LexUnsigned(Dynar<Char8> *buf);

        constexpr
        Bool PeekIsValidUnsigned()
        const noexcept { return Char::IsNumeric(this->peek) || this->peek == '_'; }

        inline
        Void SkipZeros()
        noexcept;

        inline
        Void LexSymbolic();

        inline
        Void SkipWhitespace();

        inline
        Void ResolveToken()
        noexcept;

        constexpr
        Char8 Peek(UInt64 offset = 1)
        const noexcept { return this->source[this->index + offset]; }

        constexpr
        Void Advance()
        noexcept {
            this->peek = this->Peek();
            if (this->peek == '\n') {
                this->point.Line++;
                this->point.Column = 0;
            }
            this->point.Column++;
            this->index++;
        }
    };
} // Analyzer

#endif // KPLC_COMPILER_ANALYZER_LEXER_HPP
