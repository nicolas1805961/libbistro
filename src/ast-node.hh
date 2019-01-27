#pragma once

#include <ostream>

namespace bistro
{
  enum class OpType
    {
        PLUS,
        MINUS,
        TIMES,
        DIVIDE,
        MODULO,
        POWER,
    };
    /**
    ** Abstract class ASTNode.
    **
    ** Represents a node of the AST, which you can print or evaluate.
    **/
    template <typename BigNum, typename Base>
    class ASTNode
    {
    public:

        /// The BigNum class used to represent the numbers.
        using bignum_t = BigNum;

        /// The Base class.
        using base_t = Base;

        /// Shared_ptr to a BigNum.
        using num_t = std::shared_ptr<BigNum>;

        using self_t = ASTNode;
        /// Type of a node in the AST.
        /// Keep in mind you should be able to add/delete those.
        using node_t = int;

        virtual ~ASTNode();

        /// Print the tree in infix notation, e.g. "(2+3)".
        virtual std::ostream&
        print_infix(std::ostream& out, const base_t& b) const = 0;

        /// Print the tree in polish notation, e.g. "+ 2 3".
        virtual std::ostream&
        print_pol(std::ostream&, const base_t&) const
        {
            throw "Not implemented";
        }

        /// Print the tree in reverse polish notation, e.g. "2 3 +".
        virtual std::ostream&
        print_rpol(std::ostream&, const base_t&) const
        {
            throw "Not implemented";
        }

        /// Evaluate the tree and return a shared_pointer to the result.
        virtual num_t eval() const = 0;
    };
}
