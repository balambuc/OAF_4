#include <iostream>
#include <vector>
#include <fstream>
#include "../src/seqinfileenumerator.hpp"
#include "../src/summation.hpp"

/**
 * Tranzakció típus
 * acc (string) - számlaszám
 * bal (string) - befizetés/egyenleg
 */
struct Transaction {
    std::string acc;
    int bal;

    Transaction(std::string a, int b) : acc{std::move(a)}, bal{b} {}
    Transaction() : Transaction{"", 0} {}

    friend std::istream& operator>>(std::istream& is, Transaction& ts) {
        is >> ts.acc >> ts.bal;
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const Transaction& ts) {
        os << ts.acc << " " << ts.bal;
        return os;
    }
};

/**
 * Belső összegzés
 * addig összegez amíg az adott tranzakció számlaszáma megegyezik
 */
class SumAcc : public Summation<Transaction> {
private:
    void init() override {}
    void add(const Transaction& e) override { _result->bal += e.bal;}
    bool whileCond(const Transaction& e) const override { return e.acc == _result->acc; }
public:
    explicit SumAcc(Transaction& ts) : Summation<Transaction>(&ts) {}
};

/**
 * Külső felsoroló
 * felsorolja az összegzett számlákat
 */
class myEnor : public Enumerator<Transaction> {
private:
    SeqInFileEnumerator<Transaction>* _enor;
    Transaction _current;
    bool _end;
public:
    explicit myEnor(const std::string& input) : _current{"", 0},
                                                _end{false} { _enor = new SeqInFileEnumerator<Transaction>(input); }
    ~myEnor() override { delete _enor; }
    void first() override {
        _enor->first();
        next();
    }
    void next() override {
        _end = _enor->end();
        if (!_end)
        {
            _current = _enor->current();
            SumAcc sa(_current);
            sa.addEnumerator(_enor);
            sa.run();
        }
    }
    bool end() const override { return _end; }
    Transaction current() const override { return _current; }
};

/**
 * Külső összegzés
 * kiírja outfileba a számlákat
 */
class mySum : public Summation<Transaction, std::ofstream> {
private:
    void init() override {}
    void add(const Transaction& e) override { *_result << e << std::endl;}
public:
    explicit mySum(std::ofstream& outFile) : Summation<Transaction, std::ofstream>(&outFile) {}
};


using namespace std;

int main(int argc, char** argv) {
    string infile, outfile;
    cout << "Bemeneti fajlnev: ";
    cin >> infile;
    cout << "Kimeneti fajlnev: ";
    cin >> outfile;
    myEnor enor(infile);

    ofstream of;
    of.open(outfile.c_str());

    mySum write(of);
    write.addEnumerator(&enor);
    write.run();

    return 0;
}