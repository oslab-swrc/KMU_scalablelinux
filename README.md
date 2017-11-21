## LDU(Log-based Deferred Update)

# What is LDU?

We propose a novel light weight concurrent update method, LDU, to improve performance scalability for Linux kernel on many-core systems through eliminating lock contentions for update-heavy global data structures during process spawning and optimizing update logs. The proposed LDU is implemented into Linux kernel 4.5-rc6 and evaluated using representative benchmark programs. Our evaluation reveals that the Linux kernel with LDU shows performance improvement by ranging from 1.2x through 2.2x on a 120 core system. 

Challenges to designing a deferred update mechanism includes performing concurrent update with minimal cache line transfers allowing parallel updates. At each update operation, LDU records this update operation log to lock-less list. Before the read operation, LDU applies the updates log in chronological order. In order to deferred update, LDU divide the update operation into logical update and physical update. The logical update inserts logs into the lock-less list and carries out update side absorbing; on the other hand, the physical update executes these operations that are minimized by the update side absorbing.

# How to Use

If you want to use this kernel, you can usually replace it by using general kernel compilation method.


# LICENSE

It is distributed under the GNU General Public License - see the
  accompanying COPYING file for more details. 


You can find more detail in here(https://www.actapress.com/Abstract.aspx?paperId=456169).
