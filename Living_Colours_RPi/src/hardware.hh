#ifndef HARDWARE_HH
#define HARDWARE_HH

class Buttons{
  private:
    bool last_preset_btn;
    bool last_mix_btn;
    unsigned long last_dbnc_time;

  public:
    bool preset_btn;
    bool mix_btn;

    Buttons();
    bool pollButtons();
};

class RgbLed {
  public:
    RgbLed();
    void SetRed(int value);
    void SetGreen(int value);
    void SetBlue(int value);
    void SetColour(Colour colour);
    void Dim();
};

class PotMeter {
  private:
    int minValue;
    int maxValue;
  public:
    PotMeter();
    int read();
    int map(int value, int min_out, int max_out);
};

#endif //HARDWARE_HH
