#include <iostream>
#include "lib/sqldriver/include/jdbc/mysql_driver.h"
#include "lib/sqldriver/include/jdbc/mysql_connection.h"
#include "lib/sqldriver/include/jdbc/cppconn/resultset.h"
#include "lib/sqldriver/include/jdbc/cppconn/statement.h"
#include "lib/SDL/include/SDL.h"
#include "lib/sqldriver/include/mysql/jdbc.h"

using namespace std;


// Funkcija za vzpostavitev povezave z MySQL
sql::Connection* poveziZMySQL(const string& uporabnik, const string& geslo, const string& imeBaze) {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", uporabnik, geslo);

    return con;
}

// Funkcija za vpis podatkov o izdelku v MySQL
void vpisIzdelka(sql::Connection *con, const string& sifra, const string& rokTrajanja) {
    sql::Statement *stmt;

    stmt = con->createStatement();
    stmt->execute("USE ime_baze"); // Nastavite ustrezno ime baze

    string query = "INSERT INTO izdelki (sifra, rok_trajanja) VALUES ('" + sifra + "', '" + rokTrajanja + "')";
    stmt->execute(query);

    delete stmt;
}

// Funkcija za izpis podatkov iz MySQL
void izpisBaze(sql::Connection *con) {
    sql::Statement *stmt;
    sql::ResultSet *res;

    stmt = con->createStatement();
    stmt->execute("USE ime_baze"); // Nastavite ustrezno ime baze

    res = stmt->executeQuery("SELECT * FROM izdelki");

    cout << "Podatki v bazi:" << endl;
    while (res->next()) {
        cout << "Šifra: " << res->getString("sifra") << ", Rok trajanja: " << res->getString("rok_trajanja") << endl;
    }

    delete res;
    delete stmt;
}

int main() {
    // Nastavite ustrezne podatke za povezavo z MySQL
    string uporabnik = "vas_uporabnik";
    string geslo = "vaso_geslo";
    string imeBaze = "vasa_baza";

    sql::Connection *con = poveziZMySQL(uporabnik, geslo, imeBaze);

    char izbira;

    do {
        cout << "\nIzberi možnost:\n";
        cout << "1. Vpis izdelka\n";
        cout << "2. Izpis baze\n";
        cout << "0. Izhod\n";
        cout << "Izbira: ";
        cin >> izbira;

        switch (izbira) {
            case '1': {
                string sifra, rokTrajanja;
                cout << "Vnesi šifro izdelka: ";
                cin >> sifra;
                cout << "Vnesi rok trajanja izdelka: ";
                cin >> rokTrajanja;

                vpisIzdelka(con, sifra, rokTrajanja);
                cout << "Izdelek uspešno dodan!" << endl;
                break;
            }
            case '2':
                izpisBaze(con);
                break;
            case '0':
                cout << "Izhod iz programa." << endl;
                break;
            default:
                cout << "Neveljavna izbira. Poskusite znova." << endl;
        }

    } while (izbira != '0');

    delete con;

    return 0;
}