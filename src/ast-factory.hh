#pragma once

#include <fstream>
#include <iostream>
#include <memory> // shared_ptr
#include <stdexcept> // domain_error
#include <utility> // pair

#include "ast-node.hh"

namespace bistro
{

    /**
    ** AST factory node.
    **
    ** This is an implementation of the factory pattern.
    ** A node may be constructed with one or two operand (nodes) and an operator
    ** (OpType, defined in ast-node header), or with a BigNum.
    **/
    template <typename BigNum, typename Base>
    class ASTFactory
    {
    public:
        /// BigNum.
        using num_t = BigNum;
        /// Base.
        using base_t = Base;
        /// Pointer to AST node.
        using node_t = std::shared_ptr<ASTNode<BigNum,Base>>;
        ASTFactory() = default;

        node_t operator()(const node_t& lhs, OpType op) const
        {
            /* FIXME */
            throw "Not implemented";
        }

        node_t operator()(const node_t& lhs, const node_t& rhs, OpType op) const
        {
            /* FIXME */
            throw "Not implemented";
        }

        node_t operator()(const std::shared_ptr<num_t>& num) const
        {
            /* FIXME */
            throw "Not implemented";
        }
    };
}
