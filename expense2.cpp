#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <algorithm>  // For sort, lower_bound, upper_bound
#include <iomanip>

// --- Expense Struct ---
struct Expense {
    double amount;
    std::string category;
    std::string date;  // Format: YYYY-MM-DD
    bool deleted = false;  // Lazy deletion marker

    // Comparator to sort by date
    bool operator<(const Expense& other) const {
        return date < other.date;
    }
};

// --- Comparator for Priority Queue (Max-Heap) ---
struct CompareByAmount {
    bool operator()(const Expense& a, const Expense& b) {
        return a.amount < b.amount;  // Max-Heap (largest amount at top)
    }
};

// --- Expense Tracker Class ---
class ExpenseTracker {
private:
    std::vector<Expense> expenses;  // Store all expenses
    std::unordered_map<std::string, std::vector<Expense>> categoryMap;  // Fast category filtering
    std::unordered_map<std::string, double> categoryTotals;  // Pre-computed totals by category
    std::priority_queue<Expense, std::vector<Expense>, CompareByAmount> maxHeap;  // Top expenses

public:
    // Add an expense and update relevant data structures
    void addExpense(double amount, const std::string& category, const std::string& date) {
        Expense exp{amount, category, date};
        expenses.push_back(exp);  // Add to expense list

        categoryMap[category].push_back(exp);  // Group by category
        categoryTotals[category] += amount;  // Update category total

        maxHeap.push(exp);  // Add to max-heap for tracking top expenses
    }

    // View all expenses, excluding lazily deleted ones
    void viewExpenses() const {
        std::cout << "Amount | Category | Date\n";
        std::cout << std::fixed << std::setprecision(2);
        for (const auto& exp : expenses) {
            if (!exp.deleted) {  // Skip lazily deleted expenses
                std::cout << "$" << exp.amount << " | " 
                          << exp.category << " | " 
                          << exp.date << '\n';
            }
        }
    }

    // Filter expenses by category in O(1) time
    void filterByCategory(const std::string& category) const {
        std::cout << "Expenses for category: " << category << "\n";
        std::cout << "Amount | Date\n";
        std::cout << std::fixed << std::setprecision(2);

        if (categoryMap.find(category) != categoryMap.end()) {
            for (const auto& exp : categoryMap.at(category)) {
                if (!exp.deleted) {  // Skip deleted ones
                    std::cout << "$" << exp.amount << " | " << exp.date << '\n';
                }
            }
        } else {
            std::cout << "No expenses found for this category.\n";
        }
    }

    // View total expenses for each category in O(1) time
    void viewCategoryTotals() const {
        std::cout << "Total Expenses by Category:\n";
        for (const auto& [category, total] : categoryTotals) {
            std::cout << category << ": $" << total << '\n';
        }
    }

    // Sort expenses by date and filter by a specific date using binary search
    void filterByDate(const std::string& targetDate) {
        std::sort(expenses.begin(), expenses.end());  // Sort by date

        auto range = std::equal_range(
            expenses.begin(), expenses.end(), 
            Expense{0, "", targetDate}
        );

        if (range.first == range.second) {
            std::cout << "No expenses found for date: " << targetDate << "\n";
            return;
        }

        std::cout << "Expenses for " << targetDate << ":\n";
        std::cout << "Amount | Category\n";
        for (auto it = range.first; it != range.second; ++it) {
            if (!it->deleted) {  // Skip deleted ones
                std::cout << "$" << it->amount << " | " << it->category << '\n';
            }
        }
    }

    // Delete an expense by marking it as deleted (lazy deletion)
    void deleteExpense(size_t index) {
        if (index < expenses.size()) {
            expenses[index].deleted = true;  // Mark as deleted
            std::cout << "Expense deleted lazily.\n";
        } else {
            std::cout << "Invalid index.\n";
        }
    }

    // Get the top N most expensive purchases using a max-heap
    void viewTopExpenses(size_t N) {
        std::priority_queue<Expense, std::vector<Expense>, CompareByAmount> tempHeap = maxHeap;

        std::cout << "Top " << N << " Expenses:\n";
        std::cout << "Amount | Category | Date\n";
        std::cout << std::fixed << std::setprecision(2);

        for (size_t i = 0; i < N && !tempHeap.empty(); ++i) {
            Expense top = tempHeap.top();
            tempHeap.pop();
            if (!top.deleted) {  // Skip deleted ones
                std::cout << "$" << top.amount << " | " 
                          << top.category << " | " 
                          << top.date << '\n';
            }
        }
    }
};

int main() {
    ExpenseTracker tracker;
    int choice;

    do {
        std::cout << "\n=== Expense Tracker ===\n";
        std::cout << "1. Add Expense\n";
        std::cout << "2. View Expenses\n";
        std::cout << "3. Filter by Category\n";
        std::cout << "4. View Category Totals\n";
        std::cout << "5. Filter by Date\n";
        std::cout << "6. Delete Expense\n";
        std::cout << "7. View Top N Expenses\n";
        std::cout << "8. Exit\n";
        std::cout << "========================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            double amount;
            std::string category, date;
            std::cout << "Enter amount: ";
            std::cin >> amount;
            std::cin.ignore();
            std::cout << "Enter category: ";
            std::getline(std::cin, category);
            std::cout << "Enter date (YYYY-MM-DD): ";
            std::getline(std::cin, date);
            tracker.addExpense(amount, category, date);
        } else if (choice == 2) {
            tracker.viewExpenses();
        } else if (choice == 3) {
            std::string category;
            std::cin.ignore();
            std::cout << "Enter category: ";
            std::getline(std::cin, category);
            tracker.filterByCategory(category);
        } else if (choice == 4) {
            tracker.viewCategoryTotals();
        } else if (choice == 5) {
            std::string date;
            std::cin.ignore();
            std::cout << "Enter date (YYYY-MM-DD): ";
            std::getline(std::cin, date);
            tracker.filterByDate(date);
        } else if (choice == 6) {
            size_t index;
            std::cout << "Enter index to delete: ";
            std::cin >> index;
            tracker.deleteExpense(index);
        } else if (choice == 7) {
            size_t N;
            std::cout << "Enter N: ";
            std::cin >> N;
            tracker.viewTopExpenses(N);
        }
    } while (choice != 8);

    return 0;
}
