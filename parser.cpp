#include "parser.h"
#include <stdexcept>


namespace bunny
{

    void Parser::nextToken()
    {
        currentToken = peekedToken;
        peekedToken  = lex.getNextToken();
    }

    bool Parser::expectedPeek(const tokenType &_type)
    {
        if(peekedToken.type == _type)
        {
            nextToken();
            return true;
        }
        else
        {
            peekError(_type);
            return false;
        }
    }

    precedenza Parser::peekPrec()
    {
        if(precedences.find(peekedToken.type) != precedences.end())
            return precedences[peekedToken.type];

        return LOWEST;
    }

    precedenza Parser::currentPrec()
    {
        if(precedences.find(currentToken.type) != precedences.end())
            return precedences[currentToken.type];

        return LOWEST;
    }

    void Parser::peekError(const tokenType &_type)
    {
        errors.push_back("Peeked Token Type: " + peekedToken.type + ", Expected Type: " + _type + "\n");
    }


    //Statements

    Statement *Parser::parseStatement()
    {

        if(currentToken.type == VAR) return parseVarStatement();
        else if(currentToken.type == REF) return parseRefStatement();
        else if(currentToken.type == RETURN) return parseReturnStatement();
        else return parseExpressionStatement();
    }

    //var x = 5;
    VarStatement *Parser::parseVarStatement()
    {

        //!VAR section
        VarStatement *stmt = new VarStatement();
        stmt->token = currentToken;



        //!IDENT NAME SECTION
        //We expect a variable name (IDENTIFIER)
        if(!expectedPeek(IDENTIFIER))
        {
            delete stmt;
            return nullptr;
        }



        stmt->name.token = currentToken;
        stmt->name.value = currentToken.literal;

        //!ASSIGN SECTION
        if(!expectedPeek(ASSIGN))
        {
            delete stmt;
            return nullptr;
        }

        nextToken(); //to skip =

        //!EXPRESSION EVALUATION

        stmt->value = parseExpression(LOWEST);

        if(peekedToken.type ==  SEMI) nextToken();

        return stmt;

    }

    ReferenceStatement  *Parser::parseRefStatement()
    {
        ReferenceStatement *stmt = new ReferenceStatement();
        stmt->token = currentToken;

        if(!expectedPeek(IDENTIFIER))
        {
            delete stmt;
            return nullptr;
        }

        stmt->name.token = currentToken;
        stmt->name.value = currentToken.literal;

        if(!expectedPeek(ASSIGN))
        {
            delete stmt;
            return nullptr;
        }

        nextToken();

        stmt->value = parseExpression(LOWEST);

        if(peekedToken.type == SEMI) nextToken();

        return stmt;
    }

    ReturnStatement *Parser::parseReturnStatement()
    {

        ReturnStatement *stmt = new ReturnStatement();
        stmt->token = currentToken;

        nextToken();

        stmt->returnValue = parseExpression(LOWEST);

        if(peekedToken.type == SEMI) nextToken();

        return stmt;
    }


    ExpressionStatement *Parser::parseExpressionStatement()
    {
        ExpressionStatement *stmt = new ExpressionStatement();
        stmt->token = currentToken;

        stmt->expression = parseExpression(LOWEST);

        if(peekedToken.type == SEMI) nextToken();

        return stmt;
    }

    BlockStatement *Parser::parseBlockStatement()
    {
        BlockStatement *blockStmt = new BlockStatement();
        blockStmt->token = currentToken; // {

        nextToken();

        while(currentToken.type != RBRACE && currentToken.type != END)
        {
            Statement *stmt = parseStatement();

            if(stmt != nullptr) blockStmt->statements.push_back(stmt);

            nextToken();
        }

        return blockStmt;
    }

    //!TODO: OTTIMIZZA IL CODICE
    Expression *Parser::parseExpression(precedenza _prec)
    {
        std::cout << "Current token: " + currentToken.literal << std::endl;
        //std::cout << "Peeked token: " + peekedToken.literal << std::endl;

        //Trovato carattere sconosciuto
        if(prefixParseFuncs.find(currentToken.type) == prefixParseFuncs.end())
        {
            missingPrefixFunctionError(currentToken.type);
            return nullptr;
        }

        PrefixParse _prefix = prefixParseFuncs[currentToken.type]; // NB: e' una function pointer

        Expression *leftExpression = (this->*_prefix)(); //dopo una ricerca su internet questo è il modo corretto per chiamare la funzione

        while(peekedToken.type != SEMI && _prec < peekPrec())
        {
            if(InfixParseFuncs.find(peekedToken.type) == InfixParseFuncs.end()) return leftExpression;

            InfixParse _infix = InfixParseFuncs[peekedToken.type];

            nextToken();

            leftExpression = (this->*_infix)(leftExpression);
        }

        return leftExpression;
    }

    // Tutte le espressioni che sono separate da ,
    // Utile nelle chiamate di funzioni o array
    // function(1 + 1, 2 , 3 + 3)
    std::vector<Expression *> Parser::parseExpressionList()
    {
        std::vector<Expression *> args;

        nextToken();

        args.push_back(parseExpression(LOWEST));

        while(peekedToken.type == COMMA)
        {
            nextToken(); //per skippare ,
            nextToken(); //gettare la prossima espressione

            args.push_back(parseExpression(LOWEST));

        }

        return args;
    }

    Expression *Parser::parseIdentifier()
    {
        Identifier *ident = new Identifier();

        ident->token = currentToken;
        ident->value = currentToken.literal;

        return ident;
    }

    Expression *Parser::parseBoolean()
    {
        BooleanLiteral *boolLit = new BooleanLiteral();

        boolLit->token = currentToken;
        boolLit->value = currentToken.literal == "true";

        return boolLit;
    }

    Expression *Parser::parseIntegerLiteral()
    {
        IntegerLiteral *integer = new IntegerLiteral();
        integer->token = currentToken;



        try
        {
            integer->value = std::stoi(currentToken.literal);

        }catch( const std::invalid_argument err)
        {
            errors.push_back("The integer: " + currentToken.literal + " is an invalid argument");
            return nullptr;
        }catch( const std::out_of_range err)
        {
            errors.push_back("The integer: " + currentToken.literal + " is out of range");
            return nullptr;
        }


        return integer;
    }

    Expression *Parser::parseStringLiteral()
    {
        StringLiteral *str = new StringLiteral();

        str->token = currentToken;
        str->value = currentToken.literal;

        return str;
    }

    std::vector<Identifier *> Parser::parseFunctionArguments()
    {
        std::vector<Identifier *> args;

        // Se token successivo ) vuol dire che non ci sono parametri
        if(peekedToken.type == RPAREN)
        {
            nextToken();
            return args;
        }

        nextToken(); //Skippa , o (

        Identifier * ident = new Identifier();

        ident->token = currentToken;
        ident->value = currentToken.literal;

        args.push_back(ident);

        while(peekedToken.type == COMMA)
        {
            nextToken();
            nextToken();

            Identifier *argument = new Identifier();
            argument->token = currentToken;
            argument->value = currentToken.literal;

            args.push_back(argument);

        }

        if(!expectedPeek(RPAREN)) errors.push_back("Missing ')' in call function");


        return args;
    }

    Expression *Parser::parseFunctionLiteral()
    {
        FunctionLiteral *func = new FunctionLiteral();

        func->token = currentToken; // FUNCTION

        if(!expectedPeek(LPAREN))
        {
            delete func;
            return nullptr;
        }

        func->param = this->parseFunctionArguments();

        if(!expectedPeek(LBRACE)) // { BLOCK STATEMENTS }
        {
            delete func;
            return nullptr;
        }


        func->functionBody = this->parseBlockStatement();

        return func;
    }

    Expression *Parser::parseArrayLiteral()
    {
        ArrayLiteral *arr = new ArrayLiteral();
        arr->token = currentToken;

        //Array vuoto []
        if (peekedToken.type == RBRACKET)
        {
            nextToken();
            return arr;
        }

        arr->elements = this->parseExpressionList();

        if(!expectedPeek(RBRACKET))
        {
            arr->elements = std::vector<Expression *>();
        }

        return arr;
    }

    Expression *Parser::parsePrefixExpression()
    {
        PrefixExpression *expr = new PrefixExpression();
        expr->token = currentToken;
        expr->operation = currentToken.literal;

        nextToken();

        expr->right = parseExpression(PREFIX);

        return expr;

    }

    Expression *Parser::parseGroupedExpressions()
    {
        nextToken(); //Skippa (

        Expression *expr = parseExpression(LOWEST);

        if(!expectedPeek(RPAREN))
        {
            delete expr;
            return nullptr;
        }


        return expr;
    }

    Expression *Parser::parseIfExpression()
    {
        IfExpression *_if = new IfExpression();
        _if->token = currentToken;

        if(!expectedPeek(LPAREN))
        {
            errors.push_back("ERRORE: PARENTESI SINISTRA MANCANTE DOPO L'IF");
            delete _if;
            return nullptr;
        }

        nextToken(); // skippa la parentesi sinistra

        _if->condition = parseExpression(LOWEST);

        if(!expectedPeek(RPAREN))
        {
            errors.push_back("ERRORE: PARENTESI DESTRA MANCANTE DOPO L'IF");
            delete _if;
            return nullptr;
        }

        if(!expectedPeek(LBRACE))
        {
            errors.push_back("ERRORE: MANCANTE PARENTESI GRAFFA INIZIALE NELL'IF");
            delete _if;
            return nullptr;
        }


        _if->ifBlock = parseBlockStatement();

        if(peekedToken.type == ELSE)
        {
            nextToken();

            if(!expectedPeek(LBRACE))
            {
                errors.push_back("ERRORE: MANCANTE PARENTESI GRAFFA INIZIALE NELL'ELSE");
                delete _if;
                return nullptr;
            }

            _if->elseBlock = parseBlockStatement();
        }


        return _if;
    }

    Expression *Parser::parseWhileExpression()
    {
        WhileExpression *_while = new WhileExpression();
        _while->token = currentToken;

        if(!expectedPeek(LPAREN))
        {
            errors.push_back("ERRORE: MANCANTE PARENTESI INIZIALE NEL WHILE");
            delete _while;
            return nullptr;
        }

        nextToken();

        _while->condition = parseExpression(LOWEST);

        if(!expectedPeek(RPAREN))
        {
            errors.push_back("ERRORE: MANCANTE FINALE INIZIALE NEL WHILE");
            delete _while;
            return nullptr;
        }

        if(!expectedPeek(LBRACE))
        {
            errors.push_back("ERRORE: MANCANTE PARENTESI FINALE NEL WHILE");
            delete _while;
            return nullptr;
        }

        _while->whileBlock = parseBlockStatement();

        return _while;
    }

    Expression *Parser::parseCallExpression(Expression *func)
    {
        CallExpression *_call = new CallExpression();
        _call->token = currentToken;
        _call->functionName = func;

        if(peekedToken.type == RPAREN) {nextToken(); return _call;}

        _call->arguments = parseExpressionList();

        if(!expectedPeek(RPAREN)) _call->arguments = std::vector<Expression *>(); //chiamata vuota

        return _call;
    }

    Expression *Parser::parseIndexExpression(Expression *arr)
    {
        IndexExpression *_index = new IndexExpression();
        _index->token = currentToken;
        _index->arrayVar = arr;

        nextToken();


        _index->index = parseExpression(LOWEST);

        if(!expectedPeek(RBRACKET))
        {
            errors.push_back("ERROR: MANCANTE PARENTESI QUADRATA FINALE DOPO L'ARRAY");
            delete _index;
            return nullptr;
        }

        return _index;
    }

    Expression *Parser::parseInfixExpression(Expression *left)
    {
        InfixExpression *_infix = new InfixExpression();
        _infix->token = currentToken;
        _infix->operation = currentToken.literal;
        _infix->left = left;

        precedenza _precedenza = currentPrec();

        nextToken();

        _infix->right = parseExpression(_precedenza);

        return _infix;
    }

    void Parser::missingPrefixFunctionError(tokenType type)
    {
        errors.push_back("ERRORE: NESSUNA FUNZIONE PREFIX PER " + type);
    }


    Parser::Parser(lexer& _lex)
    {
        this->lex = _lex;

        // assegnazione ai vari token della loro funzione di parsing

        //Infix

        //Operazioni
        InfixParseFuncs[PLUS] = &Parser::parseInfixExpression;
        InfixParseFuncs[MINUS] = &Parser::parseInfixExpression;
        InfixParseFuncs[SLASH] = &Parser::parseInfixExpression;
        InfixParseFuncs[ASTERISK] = &Parser::parseInfixExpression;
        InfixParseFuncs[PERCENT] = &Parser::parseInfixExpression;

        //Confronto
        InfixParseFuncs[EQ] = &Parser::parseInfixExpression;
        InfixParseFuncs[NE] = &Parser::parseInfixExpression;
        InfixParseFuncs[LT] = &Parser::parseInfixExpression;
        InfixParseFuncs[GT] = &Parser::parseInfixExpression;
        InfixParseFuncs[LE] = &Parser::parseInfixExpression;
        InfixParseFuncs[GE] = &Parser::parseInfixExpression;

        //Altro
        InfixParseFuncs[LPAREN] =   &Parser::parseCallExpression;
        InfixParseFuncs[LBRACKET] = &Parser::parseIndexExpression;


        //Prefix

        prefixParseFuncs[IDENTIFIER] =    &Parser::parseIdentifier;
        prefixParseFuncs[INT] =           &Parser::parseIntegerLiteral;
        prefixParseFuncs[STRING] =        &Parser::parseStringLiteral;
        prefixParseFuncs[BANG] =          &Parser::parsePrefixExpression;
        prefixParseFuncs[MINUS] =         &Parser::parsePrefixExpression;
        prefixParseFuncs[TRUE] =          &Parser::parseBoolean;
        prefixParseFuncs[FALSE] =         &Parser::parseBoolean;
        prefixParseFuncs[LPAREN] =        &Parser::parseGroupedExpressions;
        prefixParseFuncs[IF] =            &Parser::parseIfExpression;
        prefixParseFuncs[WHILE] =         &Parser::parseWhileExpression;
        prefixParseFuncs[FUNCTION] =      &Parser::parseFunctionLiteral;
        prefixParseFuncs[LBRACKET] =      &Parser::parseArrayLiteral;

        //per settare currentToken e peek token
        nextToken();
        nextToken();


    }


    //Entry point
    Program *Parser::parseProgram()
    {

        Program *prg = new Program();

        //Itera per il file fino a quando non raggiungi la fine
        //Selezionando le varie righe di codice
        //Le quali corrispondo al Statement
        while(currentToken.type != END)
        {

            Statement *stmt = parseStatement();

            if(stmt != nullptr)
                prg->statements.push_back(stmt);

            nextToken(); // per andare nella riga consecutiva essendo che parseStatement si ferma a SEMI
        }

        return prg;
    }
}
