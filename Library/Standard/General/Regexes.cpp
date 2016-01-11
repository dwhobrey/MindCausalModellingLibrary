#include "PlatoIncludes.h"
#include "Regexes.h"

#define PCRE_STATIC 1
#include "pcre/pcre.h"

namespace Plato {

    RegexMatcher::RegexMatcher(const string* pattern) {
        IsPatternString = true;
        PatternString = pattern;
        const char *errorMessage;
        int errorOffset;
        RegexPatternPtr = (void*)pcre_compile(pattern->c_str(),0,&errorMessage,&errorOffset,NULL);
        RegexExtraPtr = (void*)pcre_study((pcre*)RegexPatternPtr,0,&errorMessage);
    }

    RegexMatcher::RegexMatcher(const char* pattern) {
        IsPatternString = false;
        PatternCharPtr = pattern;
        const char *errorMessage;
        int errorOffset;
        RegexPatternPtr = (void*)pcre_compile(pattern,0,&errorMessage,&errorOffset,NULL);
        RegexExtraPtr = (void*)pcre_study((pcre*)RegexPatternPtr,0,&errorMessage);
    }

    RegexMatcher::~RegexMatcher() {
        pcre_free(RegexPatternPtr);
        pcre_free(RegexExtraPtr);
    }

    bool RegexMatcher::Matches(const string& text) {
        return pcre_exec((pcre*)RegexPatternPtr,(pcre_extra*)RegexExtraPtr,text.data(),(int)text.size(),0,0,NULL,0)>=0;
    }

    bool RegexMatcher::Matches(const char* text) {
        return pcre_exec((pcre*)RegexPatternPtr,(pcre_extra*)RegexExtraPtr,text,(int)strlen(text),0,0,NULL,0)>=0;
    }

    const char Regexes::RegexSpecialCharacters[] = { '.','$','^','{','[','(','|',')','*','+','?','\\' };
    const string* Regexes::WildPatternString = NULL;
    const string* Regexes::AnyPatternString = NULL;

    void Regexes::Initializer() {
        WildPatternString = new string(".+");
        AnyPatternString = new string(".*");
    }

    void Regexes::Finalizer() {
        delete WildPatternString;
        delete AnyPatternString;
    }

    bool Regexes::Matches(const char* inputText, const char* regularExpression) {
        if(inputText==NULL && regularExpression==NULL) {
            return true;
        }
        if(inputText==NULL || regularExpression==NULL) {
            return false;
        }
        RegexMatcher r(regularExpression);
        return r.Matches(inputText);
    }

    bool Regexes::Matches(const string& inputText, const char* regularExpression) {
        if(inputText.empty() && regularExpression==NULL) {
            return true;
        }
        if(inputText.empty() || regularExpression==NULL) {
            return false;
        }
        RegexMatcher r(regularExpression);
        return r.Matches(inputText);
    }

    bool Regexes::Matches(const string& inputText, const string& regularExpression) {
        if(inputText.empty() && regularExpression.empty()) {
            return true;
        }
        if(inputText.empty() || regularExpression.empty()) {
            return false;
        }
        RegexMatcher r(&regularExpression);
        return r.Matches(inputText);
    }

    bool Regexes::IsRegex(const string& inputText) {
        return inputText.find_first_of(RegexSpecialCharacters,0,sizeof(RegexSpecialCharacters))!=string::npos ;
    }

    bool Regexes::IsRegex(const char* inputText) {
        return strcspn(inputText,RegexSpecialCharacters)!=strlen(inputText);
    }
}
