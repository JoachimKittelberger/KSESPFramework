/**
 * @file KSInterfaces.h
 *
 * @brief Header file for KSInterfaces classes
 * 
 * @details 
 * 
 * @see
 * 
 * @author Joachim Kittelberger <jkittelberger@kibesoft.de>
 * @date 20.06.2022
 * @version 1.00
 *
 * @todo
 *
 * @bug
 */

/**
 * @copyright
 * MIT License
 *
 * Copyright (c) 2023 Joachim Kittelberger - KibeSoft, www.kibesoft.de
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _KSINTERFACES_H
#define _KSINTERFACES_H



//#warning IKSxxx Interfaces were former KSIxxx Interfaces. Please change Interface names in your projects.


class IKSBatteryPower {
    public:
        virtual float getVoltage() = 0;
        virtual float getBatteryPercentage() = 0;
};


class IKSTemperature {
    public:
        virtual float getTemperature() = 0;
};


class IKSHumidity {
    public:
        virtual float getHumidity() = 0;
};






// Test Verwendung von abstrakte Basis-Klassen
/*
class BaseA {
  public:
//    virtual void DoSomethingA() = 0;      // für interface als abstrakte Base class notwendig
    virtual void DoSomethingA() {};
    virtual void DoSomethingA1() {};
    virtual ~BaseA() {};   // damit destruktor sauber aufgerufen wird
};

class BaseB {
  public:
//    virtual void DoSomethingB() = 0;      // für interface als abstrakte Base class notwendig
    virtual void DoSomethingB() {};
    virtual ~BaseB() {};   // damit destruktor sauber aufgerufen wird
};

//class Component : public virtual BaseA, public virtual BaseB {    // nur notwendig, wenn gleich Basisklassen und damit mehrdeutigleit der Funktionen.
  // Dann wird nur eine Funktion übernommen, die im abgeleiteter Klasse überschrieben werden kann oder wenn schon implementiert, nicht muss.
class Component : public BaseA, public BaseB {
  public:
    //virtual void DoSomethingA() {};
    //virtual void DoSomethingB() {};
    
    virtual ~Component() {};    // damit destruktor sauber aufgerufen wird
};


void setup() {
  // put your setup code here, to run once:
  Component componente;
  componente.DoSomethingA();
  componente.DoSomethingB();

  BaseA* pBaseA = &componente;
  pBaseA->DoSomethingA();
  pBaseA->DoSomethingA1();

  BaseB* pBaseB = &componente;
  pBaseB->DoSomethingB();

  Component* pComponent = &componente;
  pComponent->DoSomethingA();
  pComponent->DoSomethingB();
  pComponent->DoSomethingA1();

}

void loop() {
  // put your main code here, to run repeatedly:

}
*/



#endif          // #define _KSINTERFACES_H
