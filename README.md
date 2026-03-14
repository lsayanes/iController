# iController Firmware

Firmware para Arduino UNO que implementa un controlador MIDI por pedales (footswitch), diseñado para operar equipos de audio/guitarra compatibles con MIDI, como los procesadores de efectos **Fractal Audio** (Axe-Fx, FM9, etc.).

---

## Descripcion

**iController** es un pedalboard MIDI construido sobre Arduino UNO. Permite enviar mensajes MIDI estándar a través del puerto serie para controlar parámetros de dispositivos externos, como la selección de escenas en un Fractal Audio Axe-Fx.

El firmware está estructurado con una jerarquía de clases en C++ que permite extender el soporte a distintos dispositivos MIDI de forma modular.

---

## Arquitectura del firmware

```
Midi  (protocolo base: baud rate, canal MIDI)
 └── Device  (abstracción de dispositivo: array de switches, escena actual)
      └── Fractal  (implementación específica para Fractal Audio)
```

### Clases principales

| Archivo | Clase | Descripcion |
|---|---|---|
| `midi.hpp` | `Midi` | Configuración base MIDI: baud rate (31250) y canal |
| `switch.hpp` | `Switch` | Representa un footswitch: pin digital y etiqueta de 2 caracteres |
| `device.hpp` | `Device` | Clase abstracta base; gestiona el array de switches dinámico |
| `Fractal.hpp` | `Fractal` | Implementación para Fractal Audio; envía CC MIDI de selección de escena |

---

## Estado del proyecto

El desarrollo actual es una **prueba de concepto** sobre **Arduino UNO**. Una vez validada la arquitectura y el protocolo MIDI, se migrará la plataforma a **ESP32**, aprovechando sus capacidades adicionales (mayor memoria, conectividad WiFi/BT, múltiples UARTs, etc.).

| Fase | Plataforma | Estado |
|---|---|---|
| Prueba de concepto | Arduino UNO (ATmega328P) | En desarrollo |
| Produccion | ESP32 | Pendiente |

### Dispositivos soportados

| Dispositivo | Estado |
|---|---|
| Fractal Audio (Axe-Fx, FM9, FM3, etc.) | En desarrollo |
| BOSS GT-1000 CORE | Pendiente |

---

## Hardware requerido

- **Arduino UNO**
- Footswitches (pulsadores momentáneos) conectados a pines digitales
- Circuito de salida MIDI (optoacoplador + conector DIN-5) conectado al pin TX (pin 1)

### Conexion MIDI

La comunicación MIDI usa el puerto `Serial` nativo del Arduino a **31250 baud**, que es el baud rate estándar MIDI.

```
Arduino TX (pin 1) --> Optoacoplador 6N138 --> Conector MIDI DIN-5
```

---

## Protocolo MIDI implementado

### Seleccion de escena (Fractal Audio)

Envía un mensaje **Control Change (CC)** en el canal configurado:

| Byte | Valor | Descripcion |
|---|---|---|
| `0xB0 \| channel` | Status byte | CC + canal MIDI |
| `34` | Control Number | Scene Select (Fractal Audio) |
| `0-7` | Value | Numero de escena |

---

## Estructura del codigo

### `iControllerFw.ino`

Punto de entrada del sketch. Instancia el dispositivo y llama a `setup()`/`loop()`.

```cpp
Fractal fractal{ 1 };  // 1 footswitch
```

### `Device` — array de switches dinamico

El array de `Switch` se aloca dinámicamente en el constructor según la cantidad de switches del dispositivo, sin usar `std::vector` ni `std::unique_ptr` (incompatibles con Arduino UNO):

```cpp
Device(const uint8_t totalSw)
  : totalSwitches{ totalSw },
    switches{ new Switch[totalSw] }
{}
```

### `Fractal::sceneDemo()`

En cada iteración del `loop()`, escanea los footswitches. Al detectar un `HIGH` en un pin, envía el siguiente número de escena por MIDI y espera a que se suelte el botón (debounce por espera activa).

---

## Agregar un nuevo dispositivo

1. Crear un nuevo header, por ejemplo `MyDevice.hpp`
2. Heredar de `Device` e implementar los métodos virtuales puros:

```cpp
class MyDevice : public Device
{
public:
  MyDevice(uint8_t totalSwitches) : Device(totalSwitches) {}

  void create() override
  {
    // configurar pines y etiquetas de cada switch
    setSwitch(0, 2, 'A', '1');
  }

  void setScene(uint8_t scene) const override
  {
    // armar y enviar mensaje MIDI
  }
};
```

3. Instanciar en el `.ino` con la cantidad de footswitches deseada.

---

## Plataforma

| Item | Detalle |
|---|---|
| Microcontrolador | ATmega328P (Arduino UNO) |
| Lenguaje | C++11 (avr-gcc) |
| Framework | Arduino |
| MIDI Baud Rate | 31250 bps |
| Alimentacion | 5V via USB o fuente externa |
