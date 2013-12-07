# PMDA++ [![Build Status](https://travis-ci.org/pcolby/pcp-pmda-cpp.png?branch=master)](https://travis-ci.org/pcolby/pcp-pmda-cpp)

PMDA++ is a header-only library that allows developers to write Performance
Metrics Domain Agents (PMDAs) for [Performance Co-Pilot
(PCP)](http://oss.sgi.com/projects/pcp/) in C++.

### What is PCP?

> [Performance Co-Pilot (PCP)](http://oss.sgi.com/projects/pcp/) is an open
source infrastructure for monitoring, visualizing, recording, responding to,
and controlling the status, activity, and performance of networks, computers,
applications, and servers.
-- <cite>[Wikipedia](https://en.wikipedia.org/wiki/Performance_Co-Pilot)</cite>

### What is a PMDA?

PCP makes use of add-ons called Performance Metrics Domain Agents (PMDAs) to
fetch performance metrics for specific domains, such as database servers,
hardware, custom applications, etc.

For more information, see the [Performance Co-Pilot Programmer's
Guide](http://oss.sgi.com/projects/pcp/doc/pcp-programmers-guide.pdf).

### What is PMDA++?

PCP includes support for writing PMDAs in C, Perl and Python.  PMDA++ is a
header-only library that allows developers to write PMDAs in C++.  It is a
light C++ wrapper around PCP's C APIs.

Here is a complete, albeit very basic, PMDA written using PMDA++ that simply
returns the current time:

```c++
#include <pcp-cpp/atom.hpp>
#include <pcp-cpp/pmda.hpp>
#include <pcp-cpp/units.hpp>

class trivial : public pcp::pmda {

public:

    virtual std::string get_pmda_name() const
    {
        return "trivial";
    }

    virtual int default_pmda_domain_number() const
    {
        return 250;
    }

protected:

    virtual pcp::metrics_description get_supported_metrics()
    {
        // trivial.time aka TRIVIAL:0:0.
        return pcp::metrics_description()(0)
            (0, "time",pcp::type<uint32_t>(), PM_SEM_COUNTER,
             pcp::units(0,1,0, 0,PM_TIME_SEC,0));
    }

    virtual fetch_value_result fetch_value(const metric_id &metric)
    {
        return pcp::atom(metric.type,time(NULL));
    }

};

int main(int argc, char *argv[])
{
    return pcp::pmda::run_daemon<trivial>(argc, argv);
}
```

Compare that to PCP's official [simple.c](http://oss.sgi.com/cgi-bin/gitweb.cgi?p=pcp/pcp.git;a=blob;f=src/pmdas/trivial/trivial.c) example.

## Find out more

Take a look at the project [wiki](https://github.com/pcolby/pcp-pmda-cpp/wiki).

## Contributing

There are lots of way you can contribute, including (but not limited to):
* reviewing the code itself (its only 1K ~ 2K lines), looking for issues such as
bugs, portability issues, and the like.
* reviewing the API, suggesting improvments such as more intuitive naming and
future-proofing.
* indentifying violations of the project's desired conventions.
* writing your own PMDA using PMDA++ to test it against real world examples.

Issues can be reported via [GitHub](https://github.com/pcolby/pcp-pmda-cpp/issues).
Pull requests are very welcome.

Mailing list coming soon.

## License

PMDA++ code is available under the [OSI-approved](http://opensource.org/licenses/BSL-1.0)
[Boost Software License](http://www.boost.org/users/license.html).

PMDA++ relies on [PCP](http://oss.sgi.com/projects/pcp/)
libraries (`libpcp` and `libpcp_pmda`), which are available under LGPL v2.1.
See [What licensing scheme does PCP
use?](http://oss.sgi.com/projects/pcp/faq.html#Q1b)
