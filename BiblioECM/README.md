# Library Management System

![C++](https://img.shields.io/badge/C++-23-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows_(Native_Console)-lightgrey)
![Database](https://img.shields.io/badge/SQLite-3-green.svg)
![Status](https://img.shields.io/badge/Status-Production-brightgreen)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## About the Project

This project is a **pro bono** software developed to support the operations of the newly inaugurated library of the **First Baptist Church of Taboão da Serra**, named **Edvaldo Cardoso Mattos Library**. It is a small community library that currently holds approximately **500 items**.

The system was designed to support the library’s core workflows, including:

- User and book registration  
- Loan, return, and renewal management  
- Title reservations  
- Enforcement of borrowing limits, blocks, and overdue rules  

---

## Architecture and Technologies

The project uses the following technologies:

- **Language:** C++23  
- **Database / Persistence:** SQLite3  

The system architecture follows these principles:

- **Paradigm:** Object-Oriented Programming with a strong focus on **Separation of Concerns (SoC)**  
- Simplified **Clean Architecture / Domain-Driven Design (DDD)**  
- **Hexagonal Architecture:** The persistence layer is fully decoupled via interfaces (ports and adapters), allowing the database implementation to be replaced with minimal impact  
- **Layer Communication:** Use of DTOs (*Data Transfer Objects*) to exchange data between the domain and the presentation layer  
- **Transaction Management (ACID):** Manual transaction control (`Begin`, `Commit`, `Rollback`) to ensure database consistency and integrity in failure scenarios  

These design choices ensure that business rules remain independent from infrastructure concerns and enable easier future evolution of the system.

---

## Context and Motivation

This is the **first software project of this scale** developed by the author. Throughout its development, the system underwent **multiple refactoring cycles** as new concepts, patterns, and best practices were learned and progressively applied.

During development, **Artificial Intelligence tools played a significant supporting role**, particularly for:

- Accelerating *boilerplate* code generation  
- Code reviews and static security analysis  
- Discussing architectural trade-offs and design decisions  

*Note: All final implementation choices and business rules were reviewed, validated, and approved by the author.*

---

## Key Learnings

The development of this system resulted in valuable learning experiences, especially in the following areas:

- Software architecture principles  
- Design and validation of entities, *value objects*, and strongly typed identifiers  
- Explicit error handling strategies, avoiding exceptions and silent failures  
- Adoption of modern C++ features such as `std::expected` and `std::optional`  

---

## Lessons Learned for Future Projects

Several challenges and design decisions made during this project serve as important lessons for future iterations or new projects:

- **UTF-8 and text encoding handling**  
  Supporting accented characters and proper encoding—inevitable in applications targeting Portuguese-speaking users—required major mid-project changes. This should have been carefully defined from the start.

- **Naming conventions**  
  The codebase contains a mix of Portuguese and English identifiers.  
  While English is the de facto standard in software development, Portuguese was intentionally preserved in some areas due to the system’s end users being native Portuguese speakers.

- **Early design of DTOs and error models**  
  DTO structures and error-handling strategies could have been better designed upfront, reducing rework and later refactoring efforts.

---

## Project Strengths

Despite being an early-stage project, the system demonstrates several notable strengths:

- Unified and explicit error handling, reducing reliance on exceptions and improving control flow clarity (with room for further refinement)  
- Database access abstracted behind interfaces, enabling decoupling from the concrete SQLite implementation and facilitating future changes or testing  
- A dedicated **staging environment**, allowing validation and testing without risking the production database  

---

## Technical Debt and Improvement Roadmap

Like most early-stage systems, this project has known limitations. The following areas have been identified for future refactoring and improvement:

### 1. Resource Optimization
- **Copy Elimination:** Reduce unnecessary object copies by favoring constant references (`const T&`) and proper use of `std::move`.  
- **Expanded use of `std::string_view`:** Increase adoption in read-only and search operations to avoid unnecessary memory allocations.

### 2. Consistency and Robustness
- **Transactional Scope:** Broaden and simplify transaction usage in multi-step operations to improve reliability and maintainability.

Minor style inconsistencies are also present, reflecting the author’s continuous learning throughout the project.

---

## Planned Enhancements

### 1. Graphical User Interface
The current interface is terminal-based (CLI) and optimized using WinAPI, prioritizing low resource usage. Planned future improvements include:
- Implementing a more user-friendly **graphical interface**, potentially using **Qt**, and  
- Removing the current dependency on Windows-specific APIs to improve portability  

### 2. Automated Notifications
Potential future features include:
- An **email notification system** (e.g., overdue reminders or reservation availability alerts), implemented via **SMTP**, possibly using libraries such as **libcurl**

---

## Final Remarks

This project represents an **important learning milestone** for the author, both technically and conceptually.

It successfully fulfills its primary goal—supporting a real-world library—while also serving as a practical case study for applying software engineering best practices in C++.

Continuous improvements and extensions are expected in future versions.

Feedback and suggestions are welcome.

---

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

The choice of the permissive MIT license reflects the project’s educational and community-driven nature, allowing students and developers to freely use, study, and modify the code.

---

## Author

Developed by **Kelvyn Emmanoel de Castro Martins Welsch de Souza**  
*Data Scientist and Quantitative Researcher (PhD Candidate – University of São Paulo)*

This open-source project serves as a case study in decoupled and maintainable C++ architectures.
