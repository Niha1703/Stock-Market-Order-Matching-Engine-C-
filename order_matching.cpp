#include <bits/stdc++.h>
using namespace std;

enum class OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    int price;
    int quantity;

    Order(int i, OrderType t, int p, int q)
        : id(i), type(t), price(p), quantity(q) {}
};

struct Trade {
    int buyId, sellId, price, quantity;
    Trade(int b, int s, int p, int q) : buyId(b), sellId(s), price(p), quantity(q) {}
};

struct BuyComparator {
    bool operator()(Order* a, Order* b) {
        return a->price < b->price; // max heap
    }
};

struct SellComparator {
    bool operator()(Order* a, Order* b) {
        return a->price > b->price; // min heap
    }
};

class OrderBook {
    priority_queue<Order*, vector<Order*>, BuyComparator> buyHeap;
    priority_queue<Order*, vector<Order*>, SellComparator> sellHeap;
    vector<Trade> trades;

public:
    void placeOrder(Order* order) {
        if (order->type == OrderType::BUY)
            buyHeap.push(order);
        else
            sellHeap.push(order);
        matchOrders();
    }

    void matchOrders() {
        while (!buyHeap.empty() && !sellHeap.empty()) {
            Order* bestBuy = buyHeap.top();
            Order* bestSell = sellHeap.top();

            if (bestBuy->price >= bestSell->price) {
                int qty = min(bestBuy->quantity, bestSell->quantity);
                int tradePrice = bestSell->price;
                trades.emplace_back(bestBuy->id, bestSell->id, tradePrice, qty);

                bestBuy->quantity -= qty;
                bestSell->quantity -= qty;

                if (bestBuy->quantity == 0) buyHeap.pop();
                if (bestSell->quantity == 0) sellHeap.pop();
            } else {
                break;
            }
        }
    }

    void printTrades() {
        if (trades.empty()) {
            cout << "No trades executed yet.\n";
        } else {
            for (auto &t : trades) {
                cout << "TRADE: BuyID=" << t.buyId
                     << ", SellID=" << t.sellId
                     << ", Price=" << t.price
                     << ", Qty=" << t.quantity << "\n";
            }
        }
    }
};

int main() {
    OrderBook book;
    int choice, id = 1;

    cout << "=== Stock Market Order Matching Engine ===\n";
    cout << "1. Place BUY Order\n2. Place SELL Order\n3. Show Trades\n4. Exit\n";

    while (true) {
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            int price, qty;
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter quantity: ";
            cin >> qty;

            OrderType type = (choice == 1) ? OrderType::BUY : OrderType::SELL;
            Order* o = new Order(id++, type, price, qty);
            book.placeOrder(o);
            cout << "Order placed successfully!\n";
        }
        else if (choice == 3) {
            book.printTrades();
        }
        else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}

