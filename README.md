# SAPHO Compilers

<div align="center">
  <img src="https://github.com/nipscernlab/nipscernweb/blob/main/assets/icons/icon_sapho_memory_128.svg" alt="SAPHO Logo" width="200"/>
</div>

## About

SAPHO Compiler is a project developed by [NIPSCERN Lab](https://nipscern.com) at the Federal University of Juiz de Fora (UFJF), Brazil, in collaboration with researchers at CERN. It consists of two compilers built using Flex and Bison with Cygwin64:

1. **CMMComp**: Translates CMM code (our custom language) to ASM instructions
2. **ASMComp**: Compiles ASM code to Verilog (.v) and generates Memory Initialization File (.mif)

This project has been widely used in research and development, particularly in the ATLAS collaboration at CERN, where it helps researchers, students, and faculty members in optimizing hardware design and simulation. The project includes automatic testbench generation and simulation visualization using Icarus Verilog and GTKWave.

<div align="center">
  <img src="https://github.com/nipscernlab/nipscernweb/blob/main/assets/background/sapho_working_github_2.png" alt="Using GTKWave for FFT simulation" width="1000"/>
  <p>Using GTKWave for visualization of FFT simulation</p>
</div>

## Prerequisites

- Cygwin64
- Flex
- Bison
- Icarus Verilog
- GTKWave
- Git

## Installation

1. Clone the repository:
```bash
git clone https://github.com/nipscernlab/sapho-p.git
cd sapho-p
```

2. Ensure the proper project structure exists (see Project Structure below)
3. Configure your processor settings:
- Set the desired clock frequency
- Configure the number of clock cycles
- Set the processor name

4. Run the compilation process:
```bash
./go_proc.bat
```

## Project Structure

```
project-folder/
├── processor_name/
│   ├── Hardware/
│   │   └── [Hardware implementation files]
│   ├── Software/
│   │   └── [Your .cmm source files go here]
│   └── Simulation/
│       └── [Simulation files]
└── Temp/
    └── [Temporary compilation files]
```

## Usage

1. Place your CMM source code in the `processor_name/Software` directory
2. Run the compilation process which will:
   - Translate CMM to ASM using CMMComp
   - Generate Verilog and MIF files using ASMComp
   - Automatically create a testbench
   - Run simulation using Icarus Verilog to generate VCD file
   - Open GTKWave to visualize the simulation results

All intermediate files will be stored in the `temp` directory during the compilation process.

## Compilation Flow

```
CMM Code (.cmm) → ASM Code (.asm) → Verilog (.v) + MIF → Testbench → Simulation (.vcd) → GTKWave
```

<div align="center"> <img src="https://github.com/nipscernlab/nipscernweb/blob/main/assets/background/sapho_working_github_1.png" alt="AURORA IDE with SAPHO" width="1000"/> <p>AURORA IDE with SAPHO</p> </div>

<div align="center"> <img src="https://github.com/nipscernlab/nipscernweb/blob/main/assets/background/sapho_working_github_3.png" alt="Version control for AURORA IDE" width="1000"/> <p>Version control for AURORA IDE</p> </div>

## Contact

For questions and support, contact us at:
- Email: contact@nipscern.com

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

Working on it!

## Acknowledgments

- NIPSCERN Lab
- CERN
- Federal University of Juiz de Fora (UFJF)
- All contributors to this project

---

Made with ❤️ by NIPSCERN Lab
