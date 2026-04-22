#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                   // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)

#define TEMPO 2000 // 2 segundo
// Define o valor do registrador MOD do TPM para configurar o período do PWM
#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
// Valores de duty cycle correspondentes a diferentes larguras de pulso
uint16_t duty_Red  = TPM_MODULE*0.750;             

int main(void)
{
    pwm_tpm_Init(TPM2, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, EDGE_PWM);

    // Inicializa o canal 0 do TPM2 para gerar sinal PWM na porta GPIOB_18
    // - modo TPM_PWM_H (nível alto durante o pulso)
    pwm_tpm_Ch_Init(TPM2, 0, TPM_PWM_H, GPIOB, 18); // vermelho
    pwm_tpm_Ch_Init(TPM2, 1, TPM_PWM_H, GPIOB, 19); // Verde
    //Pinos dos motores (PTD1 e PTD2 para hor)
    pwm_tpm_Ch_Init(TPM0, 1, TPM_PWM_H, GPIOD, 1); //PTD1 
    pwm_tpm_Ch_Init(TPM0, 2, TPM_PWM_H, GPIOD, 2); //PTD2 
    //Pinos dos motores (PTC8 e PTC9 para anti-hor)
    pwm_tpm_Ch_Init(TPM0, 4, TPM_PWM_H, GPIOC, 8); //PTC8
    pwm_tpm_Ch_Init(TPM0, 5, TPM_PWM_H, GPIOC, 9); //PTC9  s
    // Loop infinito
    while(1)
    {
        pwm_tpm_CnV(TPM2, 0, duty_Red); // Vermelho
        pwm_tpm_CnV(TPM2, 1 , 1000);
        //motor_hor
        pwm_tpm_CnV(TPM0, 1, duty_Red); //hor
        pwm_tpm_CnV(TPM0, 2, duty_Red); //hor
        pwm_tpm_CnV(TPM0, 4, 1000);     //anti-hor
        pwm_tpm_CnV(TPM0, 5, 1000);     //anti-hor
        k_msleep(TEMPO);
        pwm_tpm_CnV(TPM2, 0, 1000);
        pwm_tpm_CnV(TPM2, 1, duty_Red); // Verde
        //motor_anti-hor
        pwm_tpm_CnV(TPM0, 1, 1000);     //hor
        pwm_tpm_CnV(TPM0, 2, 1000);     //hor
        pwm_tpm_CnV(TPM0, 4, duty_Red); //anti-hor
        pwm_tpm_CnV(TPM0, 5, duty_Red); //anti-hor
        k_msleep(TEMPO);
    }
    return 0;
}   