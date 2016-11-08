import static org.junit.Assert.*;

import org.junit.Test;

import com.mhhe.clrs2e.DynamicSetElement;
import com.mhhe.clrs2e.FibHeap;
import com.mhhe.clrs2e.MaxHeapPriorityQueue;

public class MaxHeapPriorityQueueTest {
	MaxHeapPriorityQueue mhpq;
	@Test
	public void testMaxHeapPriorityQueue() {
		mhpq = new MaxHeapPriorityQueue();
		assertEquals(true, mhpq.isEmpty());
	}

	//@Test
	public void testInsert() {
		mhpq = new MaxHeapPriorityQueue();
		//MaxHeapPriorityQueue cp;
		Object t1 = mhpq.insert(new DynamicSetInteger(2));
		int current_max = 2;
		System.out.println("insert 2:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();
		
		mhpq.insert(new DynamicSetInteger(30));
		current_max = 30;
		System.out.println("insert 30:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(4));
		current_max = 30;
		System.out.println("insert 4:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(9));
		current_max = 30;
		System.out.println("insert 9:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(23));
		current_max = 30;
		System.out.println("insert 23:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(5));
		current_max = 30;
		System.out.println("insert 5:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(6));
		current_max = 30;
		System.out.println("insert 6:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(7));
		current_max = 30;
		System.out.println("insert 7:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(10));
		current_max = 30;
		System.out.println("insert 10:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		Object t10 = mhpq.insert(new DynamicSetInteger(12));
		current_max = 30;
		System.out.println("insert 12:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(19));
		current_max = 30;
		System.out.println("insert 19:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(8));
		current_max = 30;
		System.out.println("insert 8:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(1));
		current_max = 30;
		System.out.println("insert 1:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(17));
		current_max = 30;
		System.out.println("insert 17:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(23));
		current_max = 30;
		System.out.println("insert 23:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();

		mhpq.insert(new DynamicSetInteger(29));
		current_max = 30;
		System.out.println("insert 29:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();
		
		mhpq.increaseKey(t10, 18);
		current_max = 30;
		System.out.println("increase 12 to 18:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();
		
		mhpq.increaseKey(t1, 14);
		current_max = 30;
		System.out.println("increase 2 to 14:");
		assertEquals(current_max, mhpq.maximum().getKey());
		System.out.println("Max now is "+current_max);
		mhpq.printQ();
	}

	@Test
	public void testIncreaseKey() {
		testInsert();
	}

}