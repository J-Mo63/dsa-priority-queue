#ifndef _PR_QUEUE_H
#define _PR_QUEUE_H

/*
 * Some headers that may be useful.
 * The utility header is included for the
 * std::pair<X,Y> class, used below.
 */
#include <vector>
#include <list>
#include <utility>
#include <iostream>

/*
 * This class implements a priority queue ADT
 * with priorities specified in ints.
 * Lower priority values precede higher values in
 * the ordering.
 * The template type E is the element type.
 * See the tests for examples.
 */
template <typename E>
class PriorityQueue {

private:

  /*
   * Class for MinHeap internal structure.
   */
  class MinHeap {
  private:

    /*
     * Class for HeapNode - the object type this MinHeap implementation stores.
     */
    class HeapNode {
    private:
      // Private fields
      int priority;
      E value;

    public:

      /*
       * HeapNode constructor that takes a priority and a value.
       */
      HeapNode(int newPriority, E newValue) {
        priority = newPriority;
        value = newValue;
      }

      /*
       * Priority getter.
       */
      int getPriority() {
        return priority;
      }

      /*
       * Priority setter.
       */
      void setPriority(int newPriority) {
        priority = newPriority;
      }

      /*
       * Value getter.
       */
      E getValue() {
        return value;
      }
    };

    /*
     * Private helper method for swapping two MinHeap elements.
     * It uses a standard swap pattern, taking two indexes of the heap.
     */
    void swap(int indexOne, int indexTwo) {
      HeapNode tmp = heap[indexOne];
      heap[indexOne] = heap[indexTwo];
      heap[indexTwo] = tmp;
    }

    /*
     * Private helper method for calculating the location of a
     * parent node from a given index.
     */
    int getParent(int index) {
      if (index % 2 == 1) {
        return index / 2;
      }
      return (index - 1) / 2;
    }

    /*
     * Helper method that returns the highest priority instance of a particular value
     */
    int findFirst(E element) {
      for (int i = 0; i < getSize(); i++) {
        if (getValue(i) == element) {
          return i;
        }
      }
      return -1;
    }

    /*
     * Private heapify method to order the values of the entire heap.
     * Finds the value of the second last floor in the heap and works its
     * way up to the root from there, swapping elements as it goes.
     * Will do this until it is entirely in order.
     */
    void heapify() {
      bool solved = false;

      // Checks for correctness
      while (!solved) {
        solved = true;

        // Iterates up from second last floor
        for (int i = heap.size()/2; i > 0; i--) {
          int index = i-1;

          // Is left child smaller?
          if (heap[index].getPriority() > heap[index*2].getPriority()) {
            swap(index, index*2);
            solved = false;
          }

          // Is right child smaller?
          else if (heap[index].getPriority() > heap[index*2+1].getPriority()) {
            swap(index, index*2+1);
            solved = false;
          }
        }
      }
    }

    /*
     * Private upHeap method that is basically a miniaturised version of the full
     * heapify method for efficiency reasons. Only ever used when inserting new nodes.
     * Follows the node up the heap because when it finds a parent that is smaller, it
     * knows that the heap is sorted.
     */
    void upHeap() {
      int i = heap.size() - 1;
      int parent = getParent(i);

      // Is parent bigger than the node?
      while (parent != i && heap[i].getPriority() < heap[parent].getPriority()) {
        swap(i, parent);
        i = parent;
        parent = getParent(i);
      }
    }

    // Private heap vector for MinHeap class
    std::vector<HeapNode> heap;

  public:

    /*
     * MinHeap constructor that takes no paramaters.
     */
    MinHeap() {
      heap = std::vector<HeapNode>();
    }

    /*
     * Returns the size of the MinHeap, which is the same as the size of the
     * heap vector it contains.
     */
    int getSize() {
      return heap.size();
    }

    /*
     * Returns the value of the highest priority node in the MinHeap.
     * Otherwise known as the root.
     */
    E getMin() {
      return heap[0].getValue();
    }

    /*
     * Returns the value of a HeapNode at a specific index in the vector.
     * This was added because some of the PriorityQueue functionality is not
     * generally performed by the MinHeap and so I 'pushed it left' in regard
     * to the functionality.
     */
    E getValue(int index) {
      return heap[index].getValue();
    }

    /*
     * This exists for a similar reason to getValue(), allowing the PriorityQueue to 
     * view the values being iterated over.
     */
    int getPriority(int index) {
      return heap[index].getPriority();
    }

    /*
     * Finds and changes the priority of an element in MinHeap.
     */
    void changePriority(E element, int newPriority) {
      int nodeIndex = findFirst(element);
      if (nodeIndex != -1) {
        heap[nodeIndex].setPriority(newPriority);
        heapify();
      }
    }

    /*
     * Removes the highest priority value from the MinHeap and resorts.
     */
    void removeFront() {
      // Swap lowest and highest priority nodes
      swap(0, heap.size()-1);
      // Remove highest priority node
      heap.pop_back();
      heapify();
    }

    /*
     * Inserts a new element into end of the heap and resorts.
     */
    void insert(int priority, E element) {
      // Check if incoming priority is legal
      if (priority >= 0) {
        heap.push_back(HeapNode(priority, element));

        // Use up-heaping on single values for greater insertion efficiency
        upHeap();
      }
    }

    /*
     * Inserts a series of new elements into end of the heap and resorts.
     */
    void insertAll(std::vector<std::pair<int,E> > newValues) {
      for (int i = 0; i < newValues.size(); i++) {
        heap.push_back(HeapNode(newValues[i].first, newValues[i].second));
      }
      heapify();
    }

  };

  // Private MinHeap for PriorityQueue
  MinHeap minHeap;


public:

  /*
   * A constructor, if you need it.
   */
  PriorityQueue() {
    minHeap = MinHeap();
  }

  /*
   * This function adds a new element "element" to the queue
   * with priorioty "priority".
   */
  void insert(int priority, E element) {
    minHeap.insert(priority, element);
  }

  /*
   * Similar to insert, but takes a whole vector of new things to
   * add.
   */
  void insert_all(std::vector<std::pair<int,E> > new_elements) {
    minHeap.insertAll(new_elements);
  }

  /*
   * Takes the lowest priority value element off the queue, 
   * and returns it.
   */
  E remove_front() {
    if (!empty()) {
      E lowestPriorityValue = minHeap.getMin();
      minHeap.removeFront();
      return lowestPriorityValue;
    }
    return E();
  }

  /*
   * Returns the lowest priority value element in the queue, but leaves
   * it in the queue.
   */
  E peek() {
    return minHeap.getMin();
  }

  /*
   * Returns a vector containing all the elements in the queue.
   */
  std::vector<E> get_all_elements() {
    std::vector<E> elements;

    for (int i = 0; i < minHeap.getSize(); i++) {
      elements.push_back(minHeap.getValue(i));
    }

    return elements;
  }

  /*
   * Returns true if the queue contains element "element", false
   * otherwise.
   */
  bool contains(E element){
    bool found = false;
    for (int i = 0; i < minHeap.getSize(); i++) {
      if (minHeap.getValue(i) == element) {
        return true;
      }
    }
    return false;
  }

  /*
   * Returns the priority of the element that matches
   * "element". If there is more than one, return it returns
   * the lowest priority value.
   * If no element matches, return -1.
   */
  int get_priority(E element){
    int lowest = -1;
    for (int i = 0; i < minHeap.getSize(); i++) {
      if (minHeap.getValue(i) == element) {
        if (lowest == -1) {
          lowest = minHeap.getPriority(i);
        }
        else if (minHeap.getPriority(i) < lowest) {

        }
      }
    }
    return lowest;
  }

  /*
   * Returns a vector containing all the priorities.
   * The ordering of the vector should match that of the
   * return from get_all_elements().
   * That is, the priority of the element
   * get_all_elements()[i] should be get_all_prriorities()[i].
   */
  std::vector<int> get_all_priorities(){
    std::vector<int> priorities;

    for (int i = 0; i < minHeap.getSize(); i++) {
      priorities.push_back(minHeap.getPriority(i));
    }

    return priorities;
  }

  /*
   * Finds the first (in priority order) element that matches
   * "element", and changes its priority to "new_priority".
   */
  void change_priority(E element, int new_priority) {
    minHeap.changePriority(element, new_priority);
  }

  /*
   * Returns the number of elements in the queue.
   */
  int size() {
    return minHeap.getSize();
  }

  /*
   * Returns true if the queue has no elements, false otherwise.
   */
  bool empty() {
    return minHeap.getSize() == 0;
  }

};

#endif