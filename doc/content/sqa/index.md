!alert construction title=[Initialize Documentation #43 [!icon!link]](https://github.com/idaholab/falcon/issues/43) prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

!template load file=app_index.md.template app=FALCON category=falcon

!!!
Setting up SQA requires a lot of love. I've set up the basic components in the `sqa_falcon.yml` and
`sqa_reports.yml` files, but the whole system is totally broken at this point. Hence, in
`config.yml`, the SQA extension is set to inactive.
!!!
