#ifndef C7EC09AB_F494_469C_B511_6A60CD2D9BC7
#define C7EC09AB_F494_469C_B511_6A60CD2D9BC7
#include "Particle.h"
class LED
{
public:
    // Konstruktor
    LED(int pin, String name);
    // Getter-Methode für den Pin
    int getPin() const;

    // Setter-Methode für den Pin
    void setPin(int newPin);

    String getName()const;
    // Getter-Methode für den Status
    bool getStatus() const;

    // Setter-Methode für den Status
    void setStatus(bool newStatus);
    
    // Methode zum Steuern der LED
    int ledControl(LED& led, String command);

    int getLedStatus(String command) const;


private:
    int pin;
    String name;     // Pin-Nummer der LED
    bool status; // Status der LED (An/Aus)
};

#endif /* C7EC09AB_F494_469C_B511_6A60CD2D9BC7 */
