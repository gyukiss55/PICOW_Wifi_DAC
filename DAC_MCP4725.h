// DAC_MCP4725.h

#ifndef DAC_MCP4725_H
#define DAC_MCP4725_H


class DAC_MCP4725 {
	DAC_MCP4725();
public:
	bool SetBuffer(int16_t* buffer, uint32_t size);
	bool PlayBuffer() const;
	void DCTest10Sec() const;
	void GenerateSin();
    void PlaySin();
    void PlaySinDirect();
    void Loop();

private:
	static DAC_MCP4725* instance;
public:
	static DAC_MCP4725* GetInstance();
};

#endif
