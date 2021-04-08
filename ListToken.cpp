#include "Token.h"
#include "ListToken.h"

ListToken::~ListToken()
{
    for (Token *token : list)
    {
        delete token;
    }
}
