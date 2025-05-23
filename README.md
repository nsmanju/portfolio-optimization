# portfolio-optimization
```
C++ program demonstrating portfolio management using function pointers and 'using' keyword.
Type alias for a function pointer is used to represent a portfolio operation.
The portfolio operation takes a Portfolio&, a symbol, a price, and a quantity.
using PortfolioOperation = void(*)(Portfolio&, const Symbol&, Price, Quantity);
```
