#include <iostream>
#include <string>
#include <gmp.h>
#include <gmpxx.h>
#include <cmath>
#include<windows.h>
#include<wincrypt.h>


using namespace std;

struct Coordinates {
    mpz_t x;
    mpz_t y;
};

Coordinates double_point(mpz_t& x, mpz_t& y ,mpz_t a ,mpz_t p){

    Coordinates result;

    if (x == 0 && y ==0){
        result.x = 0;
        result.y = 0;
        return result;
    }

    mpz_t x3 = 3*x;

    mpz_t invert;

    mpz_t two_mul_y = 2 * y;

    mpz_invert(invert.get_mpz_t(), two_mul_y.get_mpz_t(), p.get_mpz_t());

    mpz_t s = ((3 * (x *x) + a ) * invert) % p;

    mpz_t x2 = ((s *s ) -2 * x) % p;

    mpz_t y2 = (-y + s * (x - x2)) % p;

    if (x2 < 0){
        x2 += p;
    }
    if (y2 < 0){
        y2 += p;
    }

    result.x = x2;

    result.y = y2;

    return result;


}

Coordinates add_point(mpz_t x,mpz_t y,mpz_t x2,mpz_t y2,mpz_t a,mpz_t p){
    Coordinates result;

    if (x == 0 && y == 0){
        result.x = x2;
        result.y = y2;

        return result;

    }

    if (x2 == 0 && y2 == 0){
        result.x = x;
        result.y = y;

        return result;

    }

    if (x == x2 && y == y2) {
        return double_point(x, y, a, p);
    }

    mpz_t invert;

    mpz_t x_minus_x2 = x - x2;
    mpz_invert(invert.get_mpz_t(),x_minus_x2.get_mpz_t(), p.get_mpz_t() );

    mpz_t s = ((y - y2) * invert) % p;
    mpz_t t = ((y2 * x - y * x2) * invert) % p;

    mpz_t x3 = (s *s - x - x2) % p;
    
    mpz_t y3 = (-s * (s*s - x - x2) - t) % p;

    if (x3 < 0){
        x3 += p;
    }
    if (y3 < 0){
        y3 += p;
    }

    result.x = x3;
    result.y = y3;

    return result;

}

void produit_scalaire(const mpz_t x, const mpz_t y, mpz_t scalar,const mpz_t a,const mpz_t p) {

    Coordinates temp;

    temp.x = 0;
    temp.y = 0;
    string str_scalar_binary = scalar.get_str(2);

    cout << str_scalar_binary << endl;

    for (size_t i = 0; i < str_scalar_binary.length(); ++i) {

        cout << str_scalar_binary.length() << endl;

        temp = double_point(temp.x , temp.y, a, p); 

        if (str_scalar_binary[i] == '1') {
            temp = add_point(temp.x , temp.y , x, y, a, p); 
        }
        
        string printable_x = temp.x.get_str();
        string printable_y = temp.y.get_str();
        cout << printable_x << " "<< printable_y << endl;
    }

}

int main() {
    cout << "Hello World" << endl;

    mpz_t a = 0;

    const mpz_t x("55066263022277343669578718895168534326250603453777594175500187360389116729240",10);

    const mpz_t y("32670510020758816978083085130507043184471273380659243275938904335757337482424",10);

    const mpz_t p("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");

    const mpz_t scalar("1200000000000000000000000000000000000000000000000000000000000002");

    string printable_scalar = scalar.get_str();

    cout << printable_scalar << endl;

    produit_scalaire(x, y, scalar, a, p);

    return 0;
}
