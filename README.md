# Lab 1.5: Users and Services Management

*Student:* Erick Mathew Garcia Sanchez
*Date:* 29-09-25
*Course:* Operating Systems

## Executive Summary
This lab covered user and group management, service administration with systemd, system monitoring, and automation on Linux. Users and groups were created, permissions and sudoers were configured, services like Apache2 and a custom service were managed, and troubleshooting and performance analysis were documented.

## Tasks Completed
- User and group management
- Service configuration and control
- System monitoring and log analysis
- Custom service creation
- Security configuration

## Users and Groups Created
| User | Groups | Home Directory | Shell | Purpose |
|------|--------|----------------|-------|---------|
| alice | developers, administrators | /home/alice | /bin/bash | Lead Developer |
| bob | developers | /home/bob | /bin/bash | Junior Developer |
| charlie | testers | /home/charlie | /bin/bash | QA Tester |

## Services Configured
- Apache2 web server
- Custom simple-service
- SSH service analysis
- System service dependencies

## Key Learnings


    Learned to manage users, groups, and permissions.

    Practiced controlling and troubleshooting services with systemd.

    Automated admin tasks with shell scripts.

    The most challenging part was diagnosing service errors using logs.

    Best practices: document changes, use secure passwords, and apply least privilege.


## Security Measures Implemented


    Used strong, unique passwords for all users.

    Set restrictive permissions on shared directories.

    Limited sudo access and created custom sudoers rules.

    Backed up configurations before making changes.

