# 1. Flanger - NSP project

<!-- TOC -->

- [1. Flanger - NSP project](#1-flanger---nsp-project)
  - [1.1. Specification](#11-specification)
    - [1.1.1. General Specification](#111-general-specification)
  - [1.2. Principle and Matlab Simulation](#12-principle-and-matlab-simulation)
    - [1.2.1. Basic principles and MATLAB simulation](#121-basic-principles-and-matlab-simulation)
    - [1.2.2. Simulation with Sinmulink](#122-simulation-with-sinmulink)
  - [Design Idea](#design-idea)
    - [The sequence of Signal Flow](#the-sequence-of-signal-flow)

<!-- /TOC -->

## 1.1. Specification

### 1.1.1. General Specification

The Flanger-effect is caused by a comb filter with the difference equation:

$$y[k] = x[k] + \alpha  \cdot x[k - N].$$

The frequency response of the comb filter has a cosine shape with zeros at distance fa/N. The parameter $\alpha$ determines the strength of the filter, the filter effect, and thus the strength of the flanger effect.

The flanger effect is caused by the fact that the length $N$ of the filter within the period $T$ of the flanger effect is changed linearly from a minimum to a maximum value and back again.

The length $N$ of the comb filter should accept values for which the zeros of the comb filter are located in intervals of between $50\text{Hz}$ and $1\text{kHz}$. The parameter α takes values between $-1$ and $1$. The period $T$ of the effect should be between $1\text{s}$ and $10\text{s}$ adjustable.

## 1.2. Principle and Matlab Simulation

### 1.2.1. Basic principles and MATLAB simulation

For the system of flanger-effect, the system function is $$H=\frac{Y}{X}=1+\alpha Z^{-N}.$$

In MATLAB, represent the coefficient of the numerator by creating an array with only one element, and represent that of the denominator by creating an array with two elements. *(Here, choose the gain $\alpha$ to be $0.7$ and the delay factor $N$ to be $1$)*

![20220122185954](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122185954.png)

The MATLAB command **filt** can be used to generate a discrete-time transfer function.

![20220122190028](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122190028.png)

The MATLAB command **zplane** provides the z-plane plot for discrete-time systems.

![20220122190113](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122190113.png)

The MATLAB command **freqz** returns the system responses of both magnitude and phase.

![20220122190247](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122190247.png)

The MATLAB command **impz** gives out the impulse response of the transfer function.

![20220122190329](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122190329.png)

The system responses with different delay factors and gains are shown below with a table

**Z-plane**

| Gain\Delay    | $N=1$ | $N=2$ | $N=3$ | $N=4$ |
|---------------|-------|-------|-------|-------|
| $\alpha=0.7$  |       |       |       |       |
| $\alpha=-0.7$ |       |       |       |       |
| $\alpha=1$    |       |       |       |       |
| $\alpha=-1$   |       |       |       |       |
| $\alpha=1.2$  |       |       |       |       |
| $\alpha=-1.2$ |       |       |       |       |

**Impulse response**

| Gain\Delay    | $N=1$ | $N=2$ | $N=3$ | $N=4$ |
|---------------|-------|-------|-------|-------|
| $\alpha=0.7$  |       |       |       |       |
| $\alpha=-0.7$ |       |       |       |       |
| $\alpha=1$    |       |       |       |       |
| $\alpha=-1$   |       |       |       |       |
| $\alpha=1.2$  |       |       |       |       |
| $\alpha=-1.2$ |       |       |       |       |

**Frequency response**

| Gain\Delay    | $N=1$ | $N=2$ | $N=3$ | $N=4$ |
|---------------|-------|-------|-------|-------|
| $\alpha=0.7$  |       |       |       |       |
| $\alpha=-0.7$ |       |       |       |       |
| $\alpha=1$    |       |       |       |       |
| $\alpha=-1$   |       |       |       |       |
| $\alpha=1.2$  |       |       |       |       |
| $\alpha=-1.2$ |       |       |       |       |

### 1.2.2. Simulation with Sinmulink

The Simulink simulation of the reverberation system is shown below

![20220122191050](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122191050.png)

For the component *Discrete Impulse*, the sample time is set to be $1/48000$.

The output waveform of the oscilloscope is shown below

![20220122191035](https://raw.githubusercontent.com/holdenzlL/myIHS/main/images/20220122191035.png)

## Design Idea

### The sequence of Signal Flow

From the system diagram in section 1.2.2, the sequence of signal flow is designed to be:

    1. Read in the input signal and store the value into *x*
    2. Do delay operation and get the value of *delayed_input*
    3. Multiply the value of *delayedOutput* by $\alpha$ to get the value of *gainedDelayed_input*
    4. Calculate the value of *y* by adding *gainedDelayed_input* and *x* together
    5. Output the value of *output_y*
