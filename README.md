# SAPHO Compilers

<div align="center">
  <img src="https://github.com/nipscernlab/nipscernweb/blob/main/assets/icons/icon_home_psychology.svg" alt="SAPHO Logo" width="200"/>
</div>

## About

SAPHO Compilers is a project developed by NIPSCERN Lab at the Federal University of Juiz de Fora (UFJF), Brazil. It consists of two compilers built using Flex and Bison with Cygwin64:

1. **CMMComp**: Translates CMM code (our custom language) to ASM instructions
2. **ASMComp**: Compiles ASM code to Verilog (.v) and generates Memory Initialization File (.mif)

## Prerequisites

- Cygwin64
- Flex
- Bison
- Git

## Installation

1. Clone the repository:
```bash
git clone https://github.com/nipscernlab/sapho-p.git
cd sapho-p
```

2. Ensure the following directories exist in your project:
- Hardware
- Software
- Simulation

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
sapho-p/
├── Hardware/
├── Software/
├── Simulation/
├── CMMComp/
│   └── [CMM to ASM compiler files]
└── ASMComp/
    └── [ASM to Verilog compiler files]
```

## Usage

1. Write your CMM code following the custom language specification
2. Use CMMComp to translate it to ASM
3. Use ASMComp to generate Verilog and MIF files
4. The resulting files can be used in your hardware implementation

## Contact

For questions and support, contact us at:
- Email: nipscern@contac.com

## Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- NIPSCERN Lab
- Federal University of Juiz de Fora (UFJF)
- All contributors to this project

---

Made with ❤️ by NIPSCERN Lab
