#include <linux/fs.h>
#include <linux/gps.h>
#include <linux/slab.h>
#include <linux/namei.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/errno.h>

static DEFINE_RWLOCK(cur_gloc_lock);

static struct gps_location cur_gloc;

void kget_gps_location(struct gps_location *rloc)
{
	read_lock(&cur_gloc_lock);
	memcpy(rloc, &cur_gloc, sizeof(cur_gloc));
	read_unlock(&cur_gloc_lock);

	return;
}
/*
void print_gps(struct gps_location loc)
{
    double latitude, longitude;
    latitude = loc.lat_integer + loc.lat_fractional*pow(10, -6);
    longitude = loc.lng_integer + loc.lng_fractional*pow(10, -6);
}
*/
SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location tmp_loc;

	if (current_uid() != 0)
		return -EACCES;

	if (loc == NULL)
		return -EINVAL;

	if (copy_from_user(&tmp_loc, loc, sizeof(tmp_loc)))
		return -EFAULT;

	write_lock(&cur_gloc_lock);
	memcpy(&cur_gloc, &tmp_loc, sizeof(tmp_loc));
	write_unlock(&cur_gloc_lock);
	return 0;
}

SYSCALL_DEFINE2(get_gps_location, const char __user *,
				pathname, struct gps_location __user *, loc)
{
	struct gps_location tmp_loc;
	struct path fp;
	struct inode *inode;
	int status;
	int accuracy;

	if (pathname == NULL)
		return -EINVAL;

	if (loc == NULL)
		return -EINVAL;

	status = user_path(pathname, &fp);

	if (status)
		return -EINVAL;

	inode = fp.dentry->d_inode;
	if (inode_permission(inode, MAY_READ) < 0)
		return -EACCES;

	if (inode->i_op->get_gps_location != NULL)
		status = inode->i_op->get_gps_location(inode, &tmp_loc);
	else
		return -EINVAL;
	
	if (copy_to_user(loc, &tmp_loc, sizeof(*loc)) < 0)
		return -EFAULT;

	memcpy(&accuracy, &tmp_loc.accuracy, sizeof(accuracy));
	if (accuracy == -1)
		return -ENODEV;
	return status;
}

