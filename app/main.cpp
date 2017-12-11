#include <iostream>
#include <vector>
#include <fstream>
#include "../src/seqinfileenumerator.hpp"
#include "../src/summation.hpp"

struct Transaction{
    std::string acc;
    int bal;
    Transaction(std::string a, int b) : acc{std::move(a)}, bal{b} { }
    Transaction() : Transaction{"", 0} { }
    friend std::istream& operator>>(std::istream& is, Transaction& ts) { is >> ts.acc >> ts.bal; return is;}
    friend std::ostream& operator<<(std::ostream& os, const Transaction& ts) { os << ts.acc << " " << ts.bal; return os;}
};

class SumAcc : public Summation<Transaction> {
private:
    void init() override {}
    void add(const Transaction& e) override {  _result->bal += e.bal;}
    bool whileCond(const Transaction& e) const override { return e.acc == _result->acc; }

public:
    explicit SumAcc(Transaction&& ts): Summation<Transaction>(&ts) { }
};

class myEnor : public Enumerator<Transaction> {
private:
    SeqInFileEnumerator<Transaction>* _enor;

    Transaction _current;
    bool _end;
public:
    explicit myEnor(const std::string& input) : _current{"",0}, _end{false} { _enor = new SeqInFileEnumerator<Transaction>(input); }
    ~myEnor() override { delete _enor; }
    void first() override {
        _enor->first();
        next();
    }
    void next() override {
        _end = _enor->end();
        if (!_end)
        {
            SumAcc sa(_enor->current());
            sa.addEnumerator(_enor);
            sa.run();
            _current = sa.result();
        }
    }
    bool end() const override  { return _end; }
    Transaction current() const override { return _current; }
};

class mySum : public Summation<Transaction, std::ofstream > {
private:
    void init() override {}
    void add(const Transaction& e) override {  *_result << e << std::endl;}
public:
    explicit mySum(std::ofstream& ofile) : Summation<Transaction, std::ofstream>(&ofile) { }
};

using namespace std;

int main(int argc, char** argv) {
    string infile ;
    if(argc > 1) infile = argv[1];
    else
    {
        cout << "Fájlnév: ";
        cin >> infile;
    }
    myEnor enor(infile);

    ofstream of;
    string outfile = infile;
    outfile.replace(0,2,"out");
    of.open(outfile.c_str());

    mySum write(of);
    write.addEnumerator(&enor);
    write.run();

    return 0;
}