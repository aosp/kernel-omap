/*
 * linux/drivers/block/pmd.c
 *
 * Copyright (C) 2013 Texas Instruments Inc.
 * Author: Hemant Hariyani <hemanthariyani@ti.com>
 *
 * Some code and ideas taken from other block drivers.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#define DEVICE_NAME "PMD"

#include <linux/major.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/bitops.h>
#include <linux/mutex.h>
#include <linux/slab.h>

#include <asm/setup.h>
#include <asm/pgtable.h>

#define PMD_BASE 0x84000000
#define PMD_SECTORS 0x10000

#define KERNEL_SECTOR_SIZE 512

static DEFINE_MUTEX(pmd_mutex);
static u_long *pmd_virt    = NULL;
static u_long pmd_size    = 0;
static int pmd_count         = 0;
static int chip_count       = 0;
static int list_count       = 0;
static int current_device   = -1;

static DEFINE_SPINLOCK(pmd_lock);

static struct gendisk *pmd_gendisk;

static void do_pmd_request(struct request_queue *q)
{
	struct request *req;

	req = blk_fetch_request(q);
	while (req) {
		unsigned long start = blk_rq_pos(req) << 9;
		unsigned long len  = blk_rq_cur_bytes(req);
		unsigned long addr = (unsigned long)pmd_virt + start;
		int err = 0;

		if (start + len > pmd_size) {
			pr_err(DEVICE_NAME ": bad access: block=%llu, "
					"count=%u\n",
					(unsigned long long)blk_rq_pos(req),
					blk_rq_cur_sectors(req));
			err = -EIO;
			goto done;
		}
		if (rq_data_dir(req) == READ)
			memcpy(req->buffer, (char *)addr, len);
		else
			memcpy((char *)addr, req->buffer, len);
done:
		if (!__blk_end_request_cur(req, err))
			req = blk_fetch_request(q);
	}
}

static int pmd_open(struct block_device *bdev, fmode_t mode)
{
	printk("pmd_open called\n");
	return 0;
}

	static int
pmd_release(struct gendisk *disk, fmode_t mode)
{
	mutex_lock(&pmd_mutex);
	if ( current_device == -1 ) {
		mutex_unlock(&pmd_mutex);
		return 0;
	}
	mutex_unlock(&pmd_mutex);
	/*
	 * FIXME: unmap memory
	 */

	return 0;
}

static const struct block_device_operations pmd_fops =
{
	.owner		= THIS_MODULE,
	.open		= pmd_open,
	.release	= pmd_release,
};

static struct kobject *pmd_find(dev_t dev, int *part, void *data)
{
	*part = 0;
	return get_disk(pmd_gendisk);
}

static struct request_queue *pmd_queue;

	static int __init
pmd_init(void)
{
	int ret;

	ret = -EBUSY;
	if (register_blkdev(PMD_MAJOR, DEVICE_NAME))
	{
		printk(KERN_WARNING "PMD: Cannot assign major:%d\n", PMD_MAJOR);
		goto err;
	}
	printk("PMD: block driver registered\n");

	pmd_size = PMD_SECTORS * KERNEL_SECTOR_SIZE;
	pmd_virt = ioremap(PMD_BASE, pmd_size);
	if(!pmd_virt)
	{
		printk(KERN_WARNING "PMD: Cannot map PMD memory\n");
		pmd_virt=ioremap(PMD_BASE + 0x10000000, pmd_size);
		if(!pmd_virt)
		{
			printk(KERN_WARNING "PMD: Cannot map PMD memory 1\n");
			goto out_ioremap;
		}
	}
	printk("PMD: Mapped PMD at 0x%x size:%d\n", (unsigned long) pmd_virt, pmd_size);

	ret = -ENOMEM;
	pmd_queue = blk_init_queue(do_pmd_request, &pmd_lock);
	if (!pmd_queue)
	{
		printk(KERN_WARNING "PMD: blk_init_queue failed\n");
		goto out_queue;
	}
	printk("PMD: block queue initialized\n");

	pmd_gendisk = alloc_disk(1);
	if (!pmd_gendisk)
	{
		printk(KERN_WARNING "PMD: alloc_disk failed\n");
		goto out_disk;
	}
	printk("PMD: disk allocated\n");

	pmd_gendisk->major = PMD_MAJOR;
	pmd_gendisk->first_minor = 0;
	pmd_gendisk->fops = &pmd_fops;
	sprintf(pmd_gendisk->disk_name, "pmd");

	pmd_gendisk->queue = pmd_queue;
	set_capacity(pmd_gendisk, PMD_SECTORS);
	add_disk(pmd_gendisk);
#if 0
	blk_register_region(MKDEV(PMD_MAJOR, 0), PMDMINOR_COUNT, THIS_MODULE,
			pmd_find, NULL, NULL);
#endif
	return 0;

out_disk:
out_queue:
	iounmap(pmd_virt);
out_ioremap:
	unregister_blkdev(PMD_MAJOR, DEVICE_NAME);
err:
	return ret;
}

static void __exit pmd_exit(void)
{
#if 0
	blk_unregister_region(MKDEV(PMD_MAJOR, 0), PMDMINOR_COUNT);
#endif
	unregister_blkdev(PMD_MAJOR, DEVICE_NAME);
	del_gendisk(pmd_gendisk);
	put_disk(pmd_gendisk);
	blk_cleanup_queue(pmd_queue);
	return;
}

module_init(pmd_init);
module_exit(pmd_exit);
MODULE_LICENSE("GPL");
