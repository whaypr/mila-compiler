#include "ast.hpp"


ASTNode::~ASTNode() = default;

TypeASTNode::TypeASTNode(Type type)
    : m_type(type)
{
}

ExprASTNode::~ExprASTNode() = default;

BinOpASTNode::BinOpASTNode(Token op, std::unique_ptr<ExprASTNode> lhs, std::unique_ptr<ExprASTNode> rhs)
    : m_op(op)
    , m_lhs(std::move(lhs))
    , m_rhs(std::move(rhs))
{
}

UnaryOpASTNode::UnaryOpASTNode(Token op, std::unique_ptr<ExprASTNode> expr)
    : m_op(op)
    , m_expr(std::move(expr))
{
}

LiteralASTNode::LiteralASTNode(int64_t value)
    : m_value(value)
{
}

DeclRefASTNode::DeclRefASTNode(std::string var)
    : m_var(std::move(var))
{
}

FunCallASTNode::FunCallASTNode(std::string func, std::vector<std::unique_ptr<ExprASTNode>> args)
    : m_func(std::move(func))
    , m_args(std::move(args))
{
}

StatementASTNode::~StatementASTNode() = default;

CompoundStmtASTNode::CompoundStmtASTNode(std::vector<std::unique_ptr<StatementASTNode>> statements)
    : m_statements(std::move(statements))
{
}

VarDeclASTNode::VarDeclASTNode(std::string var, std::unique_ptr<TypeASTNode> type)
    : m_var(std::move(var))
    , m_type(std::move(type))
{
}

ConstDefASTNode::ConstDefASTNode(std::string _const, std::unique_ptr<LiteralASTNode> val)
    : m_const(std::move(_const))
    , m_val(std::move(val))
{
}

AssignASTNode::AssignASTNode(std::unique_ptr<DeclRefASTNode> var, std::unique_ptr<ExprASTNode> expr)
    : m_var(std::move(var))
    , m_expr(std::move(expr))
{
}

IfASTNode::IfASTNode(std::unique_ptr<ExprASTNode> cond, std::unique_ptr<StatementASTNode> body, std::unique_ptr<StatementASTNode> elseBody)
    : m_cond(std::move(cond))
    , m_body(std::move(body))
    , m_elseBody(std::move(elseBody))
{
}

WhileASTNode::WhileASTNode(std::unique_ptr<ExprASTNode> cond, std::unique_ptr<StatementASTNode> body)
    : m_cond(std::move(cond))
    , m_body(std::move(body))
{
}

ForASTNode::ForASTNode(std::unique_ptr<AssignASTNode> init, std::unique_ptr<ExprASTNode> to, std::unique_ptr<StatementASTNode> body)
    : m_init(std::move(init))
    , m_to(std::move(to))
    , m_body(std::move(body))
{
}

ExpressionStatementASTNode::ExpressionStatementASTNode(std::unique_ptr<ExprASTNode> expr)
    : m_expr(std::move(expr))
{
}

ProcCallASTNode::ProcCallASTNode(std::string proc, std::vector<std::unique_ptr<ExprASTNode>> args)
    : m_proc(std::move(proc))
    , m_args(std::move(args))
{
}

EmptyStmtASTNode::EmptyStmtASTNode()
{
}

ProgramASTNode::ProgramASTNode(std::vector<std::unique_ptr<StatementASTNode>> statements)
    : m_statements(std::move(statements))
{
}


//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

/*

BinOp::BinOp(Token op, std::unique_ptr<Node> lhs, std::unique_ptr<Node> rhs)
    : op(op)
    , lhs(std::move(lhs))
    , rhs(std::move(rhs))
{
}

int BinOp::evaluate() const
{
    switch (op) {
    case tok_plus:
        return lhs->evaluate() + rhs->evaluate();
    case tok_minus:
        return lhs->evaluate() - rhs->evaluate();
    case tok_star:
        return lhs->evaluate() * rhs->evaluate();
    case tok_div:
        return lhs->evaluate() / rhs->evaluate();
    case tok_equal:
        return lhs->evaluate() == rhs->evaluate();
    case tok_notequal:
        return lhs->evaluate() != rhs->evaluate();
    case tok_lt:
        return lhs->evaluate() < rhs->evaluate();
    case tok_le:
        return lhs->evaluate() <= rhs->evaluate();
    case tok_gt:
        return lhs->evaluate() > rhs->evaluate();
    case tok_ge:
        return lhs->evaluate() >= rhs->evaluate();
    default:
        throw std::runtime_error("Invalid op");
    }
}

// ---------------------------------------------------------------
Value::Value(int val)
    : v(val)
{
}

int Value::evaluate() const { return v; }

// ---------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

// tady je ukázka toho, jak bych to asi mohl dělat (jak stavět AST v rámci
rekurzivního sestupu)


Prog *Program()
{
    Dekl();
    return new Prog(SlozPrikaz());
}

void Dekl()
{
    switch (Symb) {
    case kwVAR:
        DeklProm();
        Dekl();
        break;
    case kwCONST:
        DeklKonst();
        Dekl();
        break;
    default:;
    }
}

void DeklKonst()
{
    char id[MaxLenIdent];
    int hod;
    CtiSymb();
    Srovnani_IDENT(id);
    Srovnani(EQ);
    Srovnani_NUMB(&hod);
    deklKonst(id, hod);
    ZbDeklKonst();
    Srovnani(SEMICOLON);
}

void ZbDeklKonst()
{
    if (Symb == COMMA) {
        char id[MaxLenIdent];
    int hod;
    CtiSymb();
    Srovnani_IDENT(id);
    Srovnani(EQ);
    Srovnani_NUMB(&hod);
    deklKonst(id, hod);
    ZbDeklKonst();
    }
}

void DeklProm()
{
    char id[MaxLenIdent];
    CtiSymb();
    Srovnani_IDENT(id);
    deklProm(id);
    ZbDeklProm();
    Srovnani(SEMICOLON);
}

void ZbDeklProm()
{
    if (Symb == COMMA) {
        char id[MaxLenIdent];
    CtiSymb();
    19
    Srovnani_IDENT(id);
    deklProm(id);
    ZbDeklProm();
    }
}

StatmList *SlozPrikaz()
{
    Srovnani(kwBEGIN);
    Statm *p = Prikaz();
    StatmList *su = new StatmList(p, ZbPrikazu());
    Srovnani(kwEND);
    return su;
}

StatmList *ZbPrikazu()
{
    if (Symb == SEMICOLON) {
    CtiSymb();
    Statm *p = Prikaz();
    return new StatmList(p, ZbPrikazu());
    }
    20
    return 0;
}

Statm *Prikaz()
{
    switch (Symb) {
    case IDENT: {
        Var *var = new Var(adrProm(Ident),false);
        CtiSymb();
        Srovnani(ASSGN);
        return new Assign(var, Vyraz());
        }
    case kwWRITE:
        CtiSymb();
        return new Write(Vyraz());
    case kwIF: {
        CtiSymb();
        Expr *cond = Podminka();
        Srovnani(kwTHEN);
        Statm *prikaz = Prikaz();
        return new If(cond, prikaz, CastElse());
        }
    case kwWHILE: {
        Expr *cond;
        CtiSymb();
        cond = Podminka();
        Srovnani(kwDO);
        return new While(cond, Prikaz());
        }
    case kwBEGIN:
        return SlozPrikaz();
        default:
        return new Empty;
    }
}

Statm *CastElse()
{
    if (Symb == kwELSE) {
    C   tiSymb();
    return Prikaz();
    }
r   eturn 0;
}

Expr *Podminka()
{
    Expr *left = Vyraz();
    Operator op = RelOp();
    Expr *right = Vyraz();
    return new Bop(op, left, right);
}

Operator RelOp()
{
    switch (Symb) {
    case EQ:
        CtiSymb();
        return Eq;
    case NEQ:
        CtiSymb();
        return NotEq;
    case LT:
        CtiSymb();
        return Less;
    case GT:
        CtiSymb();
        return Greater;
    case LTE:
        CtiSymb();
        return LessOrEq;
    case GTE:
        CtiSymb();
        return GreaterOrEq;
    default:
        Chyba("neocekavany symbol");
    }
}

Expr *Vyraz()
{
    if (Symb == MINUS) {
        CtiSymb();
        return ZbVyrazu(new UnMinus(Term()));
    }
    return ZbVyrazu(Term());
}

Expr *ZbVyrazu(Expr *du)
{
    switch (Symb) {
    case PLUS:
        CtiSymb();
        return ZbVyrazu(new Bop(Plus, du, Term()));
    case MINUS:
        CtiSymb();
        return ZbVyrazu(new Bop(Minus, du, Term()));
    default:
        return du;
    }
}

Expr *Term()
{
    return ZbTermu(Faktor());
}

Expr *ZbTermu(Expr *du)
{
switch (Symb) {
    case TIMES:
        CtiSymb();
        return ZbTermu(new Bop(Times, du, Faktor()));
    case DIVIDE:
        CtiSymb();
        return ZbVyrazu(new Bop(Divide, du, Faktor()));
    default:
        return du;
    }
}

Expr *Faktor()
{
    switch (Symb) {
    case IDENT:
        char id[MaxLenIdent];
        Srovnani_IDENT(id);
        return VarOrConst(id);
    case NUMB:
        int hodn;
        Srovnani_NUMB(&hodn);
        return new Numb(hodn);
    case LPAR: {
        CtiSymb();
        Expr *su = Vyraz();
        Srovnani(RPAR);
        return su;
        }
    default:
        Chyba("Neocekavany symbol");
    }
}

*/
