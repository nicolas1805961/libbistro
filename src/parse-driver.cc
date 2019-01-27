#include "parse-driver.hh"
#include "parse-bistro.hh"

namespace bistro
{
  namespace parser
  {
    std::shared_ptr<ASTNode<num_t, base_t>> ParseDriver::parse()
    {
      yyin = fopen(filename_.c_str(), "r");
      if (!yyin)
        throw std::runtime_error("Could not open file.");
      yy::parser p(*this);
      p.parse();
      fclose(yyin);
      if (error_)
        throw std::runtime_error("Parsing failed.");
      return ast_;
    }

    void ParseDriver::set_ast(std::shared_ptr<ASTNode<num_t, base_t>>& ast)
    {
      ast_ = ast;
    }

    std::shared_ptr<ASTNode<num_t, base_t>> ParseDriver::get_ast() const
    {
      return ast_;
    }

    void ParseDriver::set_base(base_t& base)
    {
      base_ = base;
    }

    base_t& ParseDriver::get_base()
    {
      return base_;
    }

    const base_t& ParseDriver::get_base() const
    {
      return base_;
    }

    bool ParseDriver::get_error() const
    {
      return error_;
    }

    void ParseDriver::set_error()
    {
      error_ = true;
    }

    const std::string& ParseDriver::get_filename() const
    {
      return filename_;
    }

    const ASTFactory<num_t, base_t>& ParseDriver::get_factory() const
    {
      return fact_;
    }
  }
}
