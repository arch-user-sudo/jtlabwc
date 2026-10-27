/* jtlabwcctl — change jtlabwc's SSD close-button and snap-overlay colors.
 *
 * Usage:
 *   jtlabwcctl close '#RRGGBB'    close button on the SSD
 *   jtlabwcctl snap '#RRGGBB'     window snapping overlay
 *   jtlabwcctl accent '#RRGGBB'   both of the above
 *   jtlabwcctl '#RRGGBB'          shorthand for accent
 *   jtlabwcctl reset              return to the theme colors
 *
 * An optional #RRGGBBAA value overrides the alpha channel.  Without it, the
 * existing theme alpha is kept (the snap overlay stays translucent, inactive
 * close buttons stay dimmed, and so on).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

static void
ctl_path(char *buf, size_t n)
{
	const char *rt = getenv("XDG_RUNTIME_DIR");
	if (!rt || !rt[0]) rt = "/tmp";
	snprintf(buf, n, "%s/jtlabwc-ctl.sock", rt);
}

static int
hex_nibble(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

static int
parse_color(const char *s, unsigned *out, int *digits)
{
	if (!s)
		return 0;
	if (*s == '#')
		s++;
	else if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		s += 2;

	size_t len = strlen(s);
	if (len != 6 && len != 8)
		return 0;

	unsigned value = 0;
	for (size_t i = 0; i < len; i++) {
		int nibble = hex_nibble(s[i]);
		if (nibble < 0)
			return 0;
		value = (value << 4) | (unsigned)nibble;
	}
	*out = value;
	*digits = (int)len;
	return 1;
}

static void
usage(const char *prog)
{
	fprintf(stderr,
		"usage: %s close COLOR | snap COLOR | accent COLOR | reset\n"
		"       %s COLOR\n"
		"  close COLOR    change the SSD close-button color\n"
		"  snap COLOR     change the window snapping overlay color\n"
		"  accent COLOR   change both colors\n"
		"  COLOR          shorthand for accent\n"
		"  reset          return to the theme colors\n"
		"COLOR is #RRGGBB, RRGGBB, or #RRGGBBAA\n",
		prog, prog);
}

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	char msg[96];
	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
		usage(argv[0]);
		return 0;
	}

	if (!strcmp(argv[1], "reset")) {
		if (argc != 2) {
			usage(argv[0]);
			return 1;
		}
		snprintf(msg, sizeof(msg), "reset\n");
	} else {
		const char *command;
		const char *color_arg;
		if (!strcmp(argv[1], "close")
				|| !strcmp(argv[1], "snap")
				|| !strcmp(argv[1], "accent")) {
			if (argc != 3) {
				usage(argv[0]);
				return 1;
			}
			command = argv[1];
			color_arg = argv[2];
		} else if (argc == 2) {
			command = "accent";
			color_arg = argv[1];
		} else {
			usage(argv[0]);
			return 1;
		}

		unsigned value = 0;
		int digits = 0;
		if (!parse_color(color_arg, &value, &digits)) {
			fprintf(stderr, "%s: color must be #RRGGBB or #RRGGBBAA\n",
				argv[0]);
			return 2;
		}
		if (digits == 8) {
			snprintf(msg, sizeof(msg), "%s #%08X\n", command, value);
		} else {
			snprintf(msg, sizeof(msg), "%s #%06X\n", command, value);
		}
	}

	int fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
	if (fd < 0) {
		perror("jtlabwcctl: socket");
		return 1;
	}

	struct sockaddr_un addr = { .sun_family = AF_UNIX };
	ctl_path(addr.sun_path, sizeof(addr.sun_path));
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		fprintf(stderr,
			"jtlabwcctl: cannot connect to %s (is jtlabwc running?)\n",
			addr.sun_path);
		close(fd);
		return 1;
	}

	size_t len = strlen(msg);
	ssize_t n = send(fd, msg, len, MSG_NOSIGNAL);
	close(fd);
	return (n >= 0 && (size_t)n == len) ? 0 : 1;
}
