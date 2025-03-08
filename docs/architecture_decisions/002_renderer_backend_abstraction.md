# 2. Renderer backend abstraction

Date: 2025-03-01

## Status

Accepted

## Context

The engine is going to support multiple render API's.
Therefore we need to introduce an abstraction layer.

## Decision

- The abstraction layer should be as lightweight as possible.
  As many API-layer specific concepts should be hidden from the user, especially since there can be a lot of difference between render API's.
  We want the underlying implementation to make the best use out of the render API, rather than force the API to work with the abstraction layer.
- The user shouldn't need to spend a lot of time to get to the device calls (no heavy abstractions).
- The abstraction layer should be easy to maintain.
  A heavily abstracted layer will likely get in the way when making modifications or extending the code.
  The more underlying API's, the more important this becomes.

## Consequences

We will have a lightweight render API abstraction for the backend that is focussed on maintainability,
makes use of the API as best as possible without being constraint by the abstraction and
is easy to maintain and extend.
