// C++ program demonstrating portfolio management using function pointers and 'using' keyword.
// This version uses type aliases for clarity and includes extensive comments.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

// Type aliases using 'using' keyword
using Symbol = std::string;
using Price = double;
using Quantity = int;

// Represents a stock holding in the portfolio.
struct Stock {
    Symbol symbol; // Stock ticker symbol
    Price price;   // Current price per share
    Quantity quantity; // Number of shares held
};

// Type alias for a portfolio (vector of Stock)
using Portfolio = std::vector<Stock>;

// Calculates the total value of the portfolio.
auto calculate_total_value(const Portfolio& portfolio) -> Price {
    Price total_value = 0.0;
    for (const auto& stock : portfolio) {
        total_value += stock.price * stock.quantity;
    }
    return total_value;
}

// Prints the current portfolio and its total value.
auto print_portfolio(const Portfolio& portfolio) -> void {
    std::cout << "Current Portfolio:\n";
    for (const auto& stock : portfolio) {
        std::cout << "Symbol: " << stock.symbol << ", Price: " << stock.price
                  << ", Quantity: " << stock.quantity << "\n";
    }
    std::cout << "Total Value: $" << calculate_total_value(portfolio) << "\n";
}

// Adds shares of a stock to the portfolio, or updates if it already exists.
auto buy_stock(Portfolio& portfolio, const Symbol& symbol,
               Price price, Quantity quantity) -> void {
    for (auto& stock : portfolio) {
        if (stock.symbol == symbol) {
            stock.quantity += quantity;
            stock.price = price; // Update price to latest
            std::cout << "Bought " << quantity << " shares of " << symbol
                      << " at $" << price << ".\n";
            return;
        }
    }
    // If stock not found, add new entry
    portfolio.push_back({symbol, price, quantity});
    std::cout << "Bought " << quantity << " shares of " << symbol << " at $"
              << price << ".\n";
}

// Sells shares of a stock from the portfolio.
auto sell_stock(Portfolio& portfolio, const Symbol& symbol,
                Quantity quantity) -> void {
    for (auto& stock : portfolio) {
        if (stock.symbol == symbol) {
            if (stock.quantity >= quantity) {
                stock.quantity -= quantity;
                std::cout << "Sold " << quantity << " shares of " << symbol
                          << ".\n";
                // Remove stock if quantity drops to zero
                if (stock.quantity == 0) {
                    portfolio.erase(std::remove_if(portfolio.begin(),
                                                   portfolio.end(),
                                                   [&](const Stock& s){
                                                       return s.symbol == symbol;
                                                   }),
                                    portfolio.end());
                }
            } else {
                std::cout << "Not enough shares of " << symbol
                          << " to sell.\n";
            }
            return;
        }
    }
    std::cout << "Stock " << symbol << " not found in portfolio.\n";
}

// Type alias for a function pointer representing a portfolio operation.
// The operation takes a Portfolio&, a symbol, a price, and a quantity.
using PortfolioOperation = void(*)(Portfolio&, const Symbol&, Price, Quantity);

int main() {
    Portfolio portfolio; // The user's portfolio

    // Map commands to their corresponding portfolio operations.
    // The 'buy' operation uses buy_stock directly.
    // The 'sell' operation uses a lambda to adapt the signature.
    std::unordered_map<std::string, PortfolioOperation> operations = {
        {"buy", buy_stock},
        {"sell", [](Portfolio& portfolio, const Symbol& symbol,
                    Price /*price*/, Quantity quantity) {
                    // The price parameter is ignored for selling.
                    sell_stock(portfolio, symbol, quantity);
                }}
    };

    // Example usage of the operations map:
    operations["buy"](portfolio, "ABC", 50.0, 10);   // Buy 10 shares of ABC at $50
    operations["buy"](portfolio, "XYZ", 25.0, 20);   // Buy 20 shares of XYZ at $25
    print_portfolio(portfolio);                      // Print current portfolio

    operations["sell"](portfolio, "ABC", 0, 5);      // Sell 5 shares of ABC
    print_portfolio(portfolio);                      // Print updated portfolio

    operations["sell"](portfolio, "XYZ", 0, 20);     // Sell all shares of XYZ
    print_portfolio(portfolio);                      // Print final portfolio

    return 0;
}