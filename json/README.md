# Shared JSON Schema

This directory contains several JSON schema files that define the object formats
used for serialized communication between the Execution process and the GUI
process.

## point.schema.json

Defines a simple point with integer coordinates.

## region.schema.json

Defines a simple region by a coordinate pair and two positive integer
dimensions.

## command.schema.json

The schema for commands sent from GUI to Execution. All JSON strings sent from
GUI to Execution should be valid under this schema.

**NOTE**: 

The `params` property in this schema should be valid under one of the
below `*_params.schema.json` schema. Particularly, the value of the `command`
property should specify which.

i.e. if `"command": "transform"`, then `params: transform_obj` where
`transform_obj` is an instance of `transform_params.schema.json`.


## \*_params.schema.json

All schema matching this pattern describe objects that may be used as the value
of `params` in instances of the Command schema.

## result.schema.json

All results returned from Execution to GUI will satisfy this schema. These are
the results of any request from the GUI.

### Related Info

For more info on the schema format and validators, see
[json-schema](http://json-schema.org).
