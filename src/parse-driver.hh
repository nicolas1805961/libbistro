#pragma once

#include <memory>
#include <string>
#include <stdexcept>

#include "ast-node.hh"
#include "ast-factory.hh"
#include "base.hh"
#include "bignum.hh"

namespace bistro
{
  namespace parser
  {
    class ParseDriver
    {
    public:
      using value_t = uint32_t;
      using base_t = Base<value_t>;
      using num_t = BigNum<value_t>;

      ParseDriver(const std::string& filename)
        : filename_(filename)
      {}

      std::shared_ptr<ASTNode<num_t, base_t>> parse();
      void set_ast(std::shared_ptr<ASTNode<num_t, base_t>>& ast);
      std::shared_ptr<ASTNode<num_t, base_t>> get_ast() const;
      void set_base(base_t& base);
      base_t& get_base();
      const base_t& get_base() const;
      bool get_error() const;
      void set_error();
      const ASTFactory<num_t, base_t>& get_factory() const;
      const std::string& get_filename() const;

    private:
      std::shared_ptr<ASTNode<num_t, base_t>> ast_;
      base_t base_;
      bool error_ = false;
      const std::string filename_;
      const ASTFactory<num_t, base_t> fact_;

    };
  }
}
